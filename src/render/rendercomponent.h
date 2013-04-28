#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "EnginePrerequisites.h"

#include "game/movablecomponent.h"

namespace Ogre {
class MovableObject;
}

namespace Caelum {

class RenderLayer;

class RenderComponent : public MovableComponent {
  public:
    RenderComponent(const String& name, RenderLayer* renderLayer);
    virtual ~RenderComponent();

    //***********************************************
    // FROM MOVABLE COMPONENT
    //************************************************
    void updatePosition(const Vector3& position) {}
    void updateOrientation(const Quaternion& orientation) {}
    void updateScale(const Vector3& scale) {}

  protected:
    virtual Ogre::MovableObject* _getMovableObject() = 0;
    void _setGameObject(GameObject* obj);

    RenderLayer* mLayer;
};

}

#endif // RENDERCOMPONENT_H
