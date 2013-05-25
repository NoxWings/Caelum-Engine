#include "render/terrain.h"

#include <OGRE/Terrain/OgreTerrainGroup.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainQuadTreeNode.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreImage.h>
#include <OGRE/OgreDataStream.h>

#include "math/unitconversor.h"
#include "math/math.h"

#include "render/renderlayer.h"

#include "core/logmanager.h"

using namespace Caelum;

Terrain::Terrain(const String& name, RenderLayer *renderLayer, uint16 terrainTileSize, Real terrainWorldSize, Vector3 origin)
    : FixedComponent(name), mTextureCount(0), mTileSize(terrainTileSize), mWorldSize(terrainWorldSize), mHeightScale(1) {
    mLayer = renderLayer;
    mTextureBlending.resize(mTextureCount);

    mTerrainOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mLayer->_getSceneManager(),
                                                Ogre::Terrain::ALIGN_X_Z,
                                                mTileSize,
                                                mWorldSize);
    mTerrainGroup->setFilenameConvention(name,".dat");

    // Set Origin
    Ogre::Vector3 ov;
    UnitConversor::Vector3ToOgreVector3(origin, ov);
    mTerrainGroup->setOrigin(ov);
}

Terrain::~Terrain() {
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainOptions;
}

void Terrain::configureLight(Light *light) {
    Ogre::ColourValue ocolour;
    ColourValue colour;

    colour = mLayer->getAmbientLight();
    ocolour.r = colour.r; ocolour.g = colour.b; ocolour.b = colour.b; ocolour.a = colour.a;
    mTerrainOptions->setCompositeMapAmbient(ocolour);

    if (light && (light->getType() == Light::LT_DIRECTIONAL)) {
        // Vector3 direction from light
        Ogre::Light *ogLight = light->_getActualLight();
        mTerrainOptions->setLightMapDirection(ogLight->getDerivedDirection());
        // Diffuse colour of the light
        mTerrainOptions->setCompositeMapDiffuse(ogLight->getDiffuseColour());
    } else {
        // TODO set some diffuse color and fixed light
    }
}

void Terrain::configureImport(Real heightScale, uint8 textureCount, Real maxPixelError,
                              Real compositeMaxDistance, Real skirtSize, uint16 minBatchSize, uint16 maxBatchSize) {
    mTerrainOptions->setMaxPixelError(maxPixelError);
    mTerrainOptions->setCompositeMapDistance(compositeMaxDistance);
    mTerrainOptions->setSkirtSize(skirtSize);

    // Import data
    mHeightScale = heightScale;
    Ogre::Terrain::ImportData& defaultImp = mTerrainGroup->getDefaultImportSettings();
    defaultImp.terrainSize = mTileSize;
    defaultImp.worldSize = mWorldSize;
    defaultImp.inputScale = mHeightScale;
    defaultImp.minBatchSize = minBatchSize;
    defaultImp.maxBatchSize = maxBatchSize;

    mTextureCount = textureCount;
    defaultImp.layerList.resize(mTextureCount);
    mTextureBlending.resize(mTextureCount);
}

void Terrain::setTexture(uint8 index, Real splatWorldSize, const String &diffuseSpecularTexture, const String &normalHeightTexture) {
    if (index < mTextureCount) {
        Ogre::Terrain::ImportData& defaultImp = mTerrainGroup->getDefaultImportSettings();
        defaultImp.layerList[index].worldSize = splatWorldSize;
        defaultImp.layerList[index].textureNames.push_back(diffuseSpecularTexture);
        defaultImp.layerList[index].textureNames.push_back(normalHeightTexture);
    }
}

void Terrain::setTextureAndHeightBlend(uint8 index, Real splatWorldSize,
                                       const String &diffuseSpecularTexture,
                                       const String &normalHeightTexture,
                                       Real blendMinHeight, Real blendFadeDistance) {
    this->setTexture(index, splatWorldSize, diffuseSpecularTexture, normalHeightTexture);
    this->setTextureHeightBlend(index, blendMinHeight, blendFadeDistance);
}

void Terrain::setTile(long x, long y, const String &terrainTexture, bool forcedLoad) {
    String filename = mTerrainGroup->generateFilename(x, y);
    if(!forcedLoad && Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(),
                                                                 filename)) {
        // Load terrain from dat file
        mTerrainGroup->defineTerrain(x, y);
    } else {
        // Load and create terrain mesh from image heightmap
        Ogre::Image img;
        img.load(terrainTexture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mTerrainGroup->defineTerrain(x, y, &img);
    }
}

void Terrain::loadAllTiles() {
    // Synchronized load
    mTerrainGroup->loadAllTerrains(true);
}

void Terrain::setTextureHeightBlend(uint8 index, Real blendMinHeight, Real blendFadeDistance) {
    TextureBlendingHeightInfo &info = mTextureBlending[index];
    info.mMinHeight = blendMinHeight;
    info.mFadeDistance = blendFadeDistance;
}

void Terrain::blendTerrain() {
    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
    while (ti.hasMoreElements()) {
        LogManager::getSingletonPtr()->logMessage("Terreno detectado");
        Ogre::Terrain* terrain = ti.getNext()->instance;
        blendTileWithHeight(terrain);
    }
    // Free resources
    mTerrainGroup->freeTemporaryResources();
}

Ogre::Terrain* Terrain::getTile(long x, long y) {
    return mTerrainGroup->getTerrain(x, y);
}

void Terrain::blendTileWithHeight(Ogre::Terrain *terrain) {
    for(uint32 i = 1; i < terrain->getLayerCount();i++) {    
        LogManager::getSingletonPtr()->logMessage("Fundiendo capa");
        Ogre::TerrainLayerBlendMap *blendMap = terrain->getLayerBlendMap(i);
        // Load Texture blending info
        Real minHeight = mTextureBlending[i].mMinHeight;
        Real fadeDist = mTextureBlending[i].mFadeDistance;
        float* pBlend = blendMap->getBlendPointer();
        // Blending
        uint16 blendMapSize = terrain->getLayerBlendMapSize();
        for(uint16 y = 0; y < blendMapSize; ++y) {
            for(uint16 x = 0; x < blendMapSize; ++x) {
                // Pixel Alpha Calculation
                Real tx, ty, height, val;
                blendMap->convertImageToTerrainSpace(size_t(x), size_t(y), &tx, &ty);
                height = terrain->getHeightAtTerrainPosition(tx, ty);
                val = (height - minHeight) / fadeDist;
                val = Math::Clamp(val, Real(0), Real(1));
                *pBlend++ = val;
            }
        }
        blendMap->dirty();
        blendMap->update();
    }
}

/*void Terrain::blendTileWithAlphaSplatting(Ogre::Terrain *terrain) {
    std::string splattings[] = {"splattingmap1.png", "splattingmap2.png", "splattingmap3.png", "splattingmap4.png", "splattingmap5.png"};

    for(uint32 j = 1;j < terrain->getLayerCount();j++)
    {
        Ogre::TerrainLayerBlendMap *blendmap = terrain->getLayerBlendMap(j);

        Ogre::Image img;
        img.load(splattings[j-1], Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        uint32 blendmapsize = terrain->getLayerBlendMapSize();
        if(img.getWidth() != blendmapsize)
            img.resize(blendmapsize, blendmapsize);

        float *ptr = blendmap->getBlendPointer();

        uint32 bp = 0;
        for(uint32 bpX = 0;bpX < img.getWidth();bpX++)
        {
            for(uint32 bpY = 0;bpY < img.getHeight();bpY++)
            {
                Ogre::ColourValue cval = img.getColourAt(bpY, bpX, 0);
                ptr[bp] = static_cast<float>(cval.r);
                bp++;
            }
        }

        blendmap->dirty();
        blendmap->update();
    }
}*/

