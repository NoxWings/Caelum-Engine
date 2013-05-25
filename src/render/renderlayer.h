#ifndef RENDERLAYER_H
#define RENDERLAYER_H

#include "EnginePrerequisites.h"

#include "game/gamelayer.h"
#include "render/colourvalue.h"
#include "render/cameracomponent.h"
#include "render/entity.h"
#include "render/light.h"
#include "render/realisticsky.h"
#include "render/terrain.h"
#include "core/resourcemanager.h"

// Forward declaration of ogrescenemanager
namespace Ogre {
class SceneManager;
class ShadowCameraSetup;
}

namespace Caelum {

enum ShadowProjectionType {
    SHADOW_PROJECTION_DEFAULT,
    SHADOW_PROJECTION_FOCUSED,
    SHADOW_PROJECTION_LISPSM,
    SHADOW_PROJECTION_PSSM
};

enum ShadowTechnique {
    SHADOWTYPE_NONE = 0x00,
    SHADOWDETAILTYPE_ADDITIVE = 0x01,
    SHADOWDETAILTYPE_MODULATIVE = 0x02,
    SHADOWDETAILTYPE_INTEGRATED = 0x04,
    SHADOWDETAILTYPE_STENCIL = 0x10,
    SHADOWDETAILTYPE_TEXTURE = 0x20,

    SHADOWTYPE_STENCIL_MODULATIVE = 0x12,
    SHADOWTYPE_STENCIL_ADDITIVE = 0x11,
    SHADOWTYPE_TEXTURE_MODULATIVE = 0x22,
    SHADOWTYPE_TEXTURE_ADDITIVE = 0x21,

    SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED = 0x25,
    SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED = 0x26
};

class RenderLayer : public GameLayer {
  public:
    RenderLayer(const String& name, const String typeName);
    virtual ~RenderLayer();

    Component* createComponentByTypeName(const String& name, const String& typeName);

    /// MOVABLE COMPONENTS
    CameraComponent* createCamera(const String& name);
    Light* createLight(const String& name, Light::LightType type = Light::LT_POINT);
    Entity* createEntity(const String& name, const String& mesh);

    /// FIXED COMPONENTS
    Terrain* createTerrain(const String& name,
                           uint16 terrainTileSize, Real terrainWorldSize,
                           Vector3 origin = Vector3::ZERO);
    RealisticSky* createRealisticSky(const String& name);

    void setSkyBox(bool enable, const String& materialName, Real distance=5000,
                   bool drawFirst=true, const Quaternion &orientation=Quaternion::IDENTITY,
                   const String& groupName=ResourceManager::DEFAULT_RESOURCE_GROUP_NAME);
    void setSkyDome(bool enable, const String& materialName, Real curvature = 10,
                    Real tiling = 8, Real distance = 4000, bool drawFirst = true,
                    const Quaternion& orientation = Quaternion::IDENTITY,
                    int xsegments = 16, int ysegments = 16, int ysegments_keep = -1,
                    const String& groupName = ResourceManager::DEFAULT_RESOURCE_GROUP_NAME);
    // createOceanSimulator
    // createSimpleWater // for pools etc

    /// RENDER SHADOWING
    void setAmbientLight(const ColourValue &color);
    const ColourValue& getAmbientLight();
    void setShadowTechnique(ShadowTechnique technique);
    ShadowTechnique getShadowTechnique();
    void setShadowColour (const ColourValue &colour);
    const ColourValue& getShadowColour();
    void setShadowDirectionalLightExtrusionDistance(Real dist);
    const Real getShadowDirectionalLightExtrusionDistance();
    void setShadowFarDistance(Real distance);
    const Real getShadowFarDistance();
    //void setShadowTextureCountPerLightType();
    //int getShadowTextureCountPerLightType();
    void setShadowTextureSettings(unsigned short size, unsigned short count=1);
    void setShadowCasterRenderBackFaces(bool bf);
    const bool getShadowCasterRenderBackFaces();
    void setShadowProjectionType (ShadowProjectionType type=SHADOW_PROJECTION_DEFAULT);
    const ShadowProjectionType getShadowProjectionType ();

    /// LAYER UPDATE
    void update(Real deltaTime);
    Ogre::SceneManager* _getSceneManager() {return _mScene;}

  private:
    // remember to use addComponent(Component* component); to register a new component
    Ogre::SceneManager *_mScene;
    ColourValue mAmbientColour, mShadowColour;
    ShadowProjectionType mShadowCameraType;
    Ogre::ShadowCameraSetup *_mShadowCamera;
};

}

#endif // RENDERLAYER_H
