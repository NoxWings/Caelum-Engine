#include "physics/btogre/bulletrigidbody.h"

#include "math/unitconversor.h"

using namespace Caelum;

BulletRigidBody::BulletRigidBody(const String &name, Real mass, BtOgre::RigidBodyState* motionState,
                                 btCollisionShape* collisionShape, const btVector3& localInertia)
    : RigidBody(name), btRigidBody(mass, motionState, collisionShape, localInertia), mMotionState(motionState){
    mMass = mass;
}

BulletRigidBody::~BulletRigidBody() {
}

void BulletRigidBody::applyImpulse(const Vector3 &impulse) {
    this->applyImpulse(impulse.x, impulse.y, impulse.z);
}

void BulletRigidBody::applyImpulse(Real x, Real y, Real z) {
    this->activate(true);
    this->applyCentralImpulse(btVector3(x,y,z));
}

void BulletRigidBody::updatePosition(const Vector3 &position) {
    this->activate(true);
    m_worldTransform.setOrigin(UnitConversor::toBullet(position));
}

void BulletRigidBody::updateOrientation(const Quaternion &orientation) {
    this->activate(true);
    m_worldTransform.setRotation(UnitConversor::toBullet(orientation));
}

void BulletRigidBody::updateScale(const Vector3 &scale) {
    this->activate(true);
    m_collisionShape->setLocalScaling(UnitConversor::toBullet(scale));
}

void BulletRigidBody::setFixedYaw() {
    this->setAngularFactor(0);
}

void BulletRigidBody::_setGameObject(GameObject *obj) {
    mGameObject = obj;
    // Make the real physics connection
    this->activate(true);
    mMotionState->setGameObject(obj);
}
