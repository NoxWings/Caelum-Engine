#ifndef BULLETRIGIDBODY_H
#define BULLETRIGIDBODY_H

#include "EnginePrerequisites.h"

#include "physics/rigidbody.h"
#include "physics/btogre/BtOgrePG.h"

// Bullet
#include <bullet/btBulletDynamicsCommon.h>

namespace Caelum {

class BulletRigidBody :
        public RigidBody,
        public btRigidBody {
  public:
    BulletRigidBody(const String &name, Real mass, BtOgre::RigidBodyState* motionState, btCollisionShape* collisionShape, const btVector3& localInertia);
    virtual ~BulletRigidBody();

    void applyImpulse(const Vector3& impulse);
    void applyImpulse(Real x, Real y, Real z);

    void updatePosition(const Vector3& position);
    void updateOrientation(const Quaternion& orientation);
    void updateScale(const Vector3& scale);

    void setFixedYaw();

protected:
    virtual void _setGameObject(GameObject* obj);

    BtOgre::RigidBodyState* mMotionState;
    Real mMass;
};

}

#endif // BULLETRIGIDBODY_H
