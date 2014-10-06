#ifndef TERRAIN_H
#define TERRAIN_H

#include "EnginePrerequisites.h"

#include "game/fixedcomponent.h"
#include "math/vector3.h"
#include "render/light.h"

#include <vector>

namespace Ogre {
class TerrainGlobalOptions;
class TerrainGroup;
class Terrain;
class TerrainQuadTreeNode;
class Renderable;
}

namespace Caelum {

class RenderLayer;

struct TextureBlendingHeightInfo {
    TextureBlendingHeightInfo()
        : mMinHeight(0), mFadeDistance(-1) {}
    TextureBlendingHeightInfo(Real minHeight, Real mFadeDistance)
        : mMinHeight(mMinHeight), mFadeDistance(mFadeDistance) {}
    TextureBlendingHeightInfo(const TextureBlendingHeightInfo& tb)
        : mMinHeight(tb.mMinHeight), mFadeDistance(tb.mFadeDistance) {}

    Real mMinHeight;
    Real mFadeDistance;
};

class Terrain : public FixedComponent {
  public:
    typedef std::vector<TextureBlendingHeightInfo> TextureBlendingHeightInfoList;
    enum TERRAIN_BLEND_TECHNIQUE {
        TERRAIN_BLEND_HEIGHT,
        TERRAIN_BLEND_ALPHASPLAT
    };

  public:
    Terrain(const String& name,
            RenderLayer* renderLayer,
            uint16 terrainTileSize, Real terrainWorldSize,
            Vector3 origin = Vector3::ZERO);
    virtual ~Terrain();

    // CONFIGURATION
    void configureLight(Light *light = NULL);
    void configureImport(Real heightScale=600,
                         uint8 textureCount = 3,
                         Real maxPixelError=8,
                         Real compositeMaxDistance=3000,
                         Real skirtSize = 10,
                         uint16 minBatchSize = 33,
                         uint16 maxBatchSize = 65);
    // TEXTURE
    void setTexture(uint8 index, Real splatWorldSize,
                    const String &diffuseSpecularTexture,
                    const String &normalHeightTexture);
    void setTextureAndHeightBlend(uint8 index, Real splatWorldSize,
                                  const String &diffuseSpecularTexture,
                                  const String &normalHeightTexture,
                                  Real blendMinHeight, Real blendFadeDistance);

    // TERRAIN TILE
    void loadTile(long x, long y, const String &terrainFile);
    void setTile(long x, long y, const String &terrainTexture, bool forcedLoad = false);
    void loadAllTiles();

    // BLENDING
    void setTextureHeightBlend(uint8 index, Real blendMinHeight, Real blendFadeDistance);
    void blendTerrain();

    Ogre::Terrain *getTile(long x, long y);
  private:
    void blendTileWithHeight(Ogre::Terrain *terrain);
    //void blendTileWithAlphaSplatting(Ogre::Terrain *terrain);

    uint8 mTextureCount;
    TextureBlendingHeightInfoList mTextureBlending;
    uint16 mTileSize;
    Real mWorldSize;
    Real mHeightScale;

    RenderLayer* mLayer;
    Ogre::TerrainGlobalOptions* mTerrainOptions;
    Ogre::TerrainGroup* mTerrainGroup;
};

}

#endif // TERRAIN_H
