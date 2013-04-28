#ifndef MOVABLECOMPONENT_H
#define MOVABLECOMPONENT_H

#include "EnginePrerequisites.h"

#include "game/component.h"
#include "game/gameobject.h"

namespace Caelum {

class MovableComponent : public Component {
  public:
    MovableComponent(const String& name);
    virtual ~MovableComponent();

    bool isAttached() {return (mGameObject!=0);}
    GameObject* getGameObject() { return mGameObject;}
    void attachTo(GameObject *obj);
    void detachFromGameObject();

    virtual void updatePosition(const Vector3& position) = 0;
    virtual void updateOrientation(const Quaternion& orientation) = 0;
    virtual void updateScale(const Vector3& scale) = 0;

    friend class GameObject;
  protected:
    virtual void _setGameObject(GameObject* obj);
    GameObject *mGameObject;
};

}

#endif // MOVABLECOMPONENT_H
