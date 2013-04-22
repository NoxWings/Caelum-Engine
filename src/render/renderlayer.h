#ifndef RENDERLAYER_H
#define RENDERLAYER_H

#include "EnginePrerequisites.h"

#include "game/gamelayer.h"

// Forward declaration of ogrescenemanager
namespace Ogre {
class SceneManager;
}

namespace Caelum {

class RenderLayer : public GameLayer {
  public:
    RenderLayer(const String& name, const String typeName);
    virtual ~RenderLayer();

    Component* createComponentByTypeName(const String& name, const String& typeName);

    /// MOVABLE COMPONENTS
    // TODO
    // createCamera
    // createLight
    // createEntity

    /// FIXED COMPONENTS
    // TODO
    // createTerrain
    // createSkySimulator
    // createSkyBox
    // createSkyDome
    // createSkyPlane
    // createOceanSimulator
    // createSimpleWater // for pools etc

    void update(Real deltaTime);

    Ogre::SceneManager* _getSceneManager() {return _mScene;}

  private:
    // remember to use addComponent(Component* component); to register a new component
    Ogre::SceneManager *_mScene;
};

}

#endif // RENDERLAYER_H
