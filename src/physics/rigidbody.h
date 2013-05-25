#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "EnginePrerequisites.h"

#include "game/movablecomponent.h"

namespace Caelum {

class RigidBody : public MovableComponent {
  public:
    RigidBody(const String &name);
    virtual ~RigidBody();

    virtual void applyImpulse(const Vector3& impulse) = 0;
    virtual void applyImpulse(Real x, Real y, Real z) = 0;

    virtual void updatePosition(const Vector3& position) = 0;
    virtual void updateOrientation(const Quaternion& orientation) = 0;
    virtual void updateScale(const Vector3& scale) = 0;

  protected:
    virtual void _setGameObject(GameObject* obj) = 0;
};

}

#endif // RIGIDBODY_H
