#include "physicslayer.h"

// Ogre
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/Terrain/OgreTerrain.h>

// Bullet
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

// BtOgre
#include "physics/btogre/BtOgrePG.h"
#include "physics/btogre/BtOgreGP.h"
#include "physics/btogre/BtOgreExtras.h"

#include "physics/btogre/bulletrigidbody.h"
#include "math/unitconversor.h"

#include "core/logmanager.h"

using namespace Caelum;

PhysicsLayer::PhysicsLayer(const String& name, const String& typeName)
    : GameLayer(name, typeName), mDebugDrawer(0), mTimeScale(1), mStepTime(0) {
    // Bullet initialization
    mBroadphase = new btAxisSweep3(btVector3(-10000,-10000,-10000), btVector3(10000,10000,10000), 1024);
    //mBroadphase = new btDbvtBroadphase();

    mCollisionConfig = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfig);
    mSolver = new btSequentialImpulseConstraintSolver();

    mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfig);
    mWorld->setGravity(btVector3(0,-10,0));
    mDebugDrawer = 0;
}

PhysicsLayer::~PhysicsLayer() {
    this->destroyAllComponents();
    delete mDebugDrawer;
    delete mWorld;
    delete mSolver;
    delete mDispatcher;
    delete mCollisionConfig;
    delete mBroadphase;
}

RigidBody* PhysicsLayer::createRigidBody(const String& name, Entity *ent, const Vector3 &scale,
                                         Real mass, PHY_SHAPE_TYPE shapeType) {
    // 1.- Create collision shape
    BtOgre::StaticMeshToShapeConverter converter(ent->_getEntity());
    btCollisionShape *collisionShape;
    switch (shapeType) {
    case PHY_SHAPE_BOX:
        collisionShape = converter.createBox();
        break;
    case PHY_SHAPE_CONVEX:
        collisionShape = converter.createConvex();
        break;
    case PHY_SHAPE_CYLINDER:
        collisionShape = converter.createCylinder();
        break;
    case PHY_SHAPE_SPHERE:
        collisionShape = converter.createSphere();
        break;
    case PHY_SHAPE_TRIMESH:
        collisionShape = converter.createTrimesh();
        break;
    default:
        collisionShape = converter.createConvex();
        break;
    }
    // 2.- Set the scale
    collisionShape->setLocalScaling( UnitConversor::toBullet(scale) );
    // 3.- Calculate inertia.
    btVector3 inertia;
    collisionShape->calculateLocalInertia(mass, inertia);
    // 4.- Create BtOgre MotionState (connects Ogre and Bullet).
    BtOgre::RigidBodyState *motionState = new BtOgre::RigidBodyState();
    // 5.- Create the Body.
    BulletRigidBody *rigidBody = new BulletRigidBody(name, mass, motionState, collisionShape, inertia);
    rigidBody->setFriction(1);
    // 6.- Register rigidBody
    mWorld->addRigidBody(rigidBody);
    this->addComponent(rigidBody);
    // 7.- Return the rigidbody
    return rigidBody;
}

void PhysicsLayer::createStaticTerrain(Terrain *terrain) {
    String name = "Terrain0x0";
    Ogre::Terrain *tile = terrain->getTile(0,0);
    // 1.- Create collision shape
#ifndef CAELUM_USE_DOUBLE
    PHY_ScalarType dataType = PHY_FLOAT;
#else
    PHY_ScalarType dataType = PHY_DOUBLE;
#endif
    bool flipQuadEdges = false;
    int upIndex = 1;
    uint16 size = tile->getSize();
    Real minH = tile->getMinHeight();
    Real maxH = tile->getMaxHeight();

    float *data = tile->getHeightData();
    float *dataConvert = new float[size * size];
    for(int i=0;i<tile->getSize();i++)
        memcpy(dataConvert + size * i, // source
               data + size * (size-i-1), // target
               sizeof(float)*(size) /*size*/ );

    btHeightfieldTerrainShape *collisionShape =
            new btHeightfieldTerrainShape(size, size, dataConvert, 1,
                                          minH, maxH, upIndex,
                                          dataType, flipQuadEdges);
    // 2.- Set shape paremeters
    Real scale = tile->getWorldSize()/(size-1);
    collisionShape->setLocalScaling(btVector3(scale, 1, scale));
    collisionShape->setUseZigzagSubdivision(true);
    LogManager::getSingletonPtr()->logMessage("CREATED TERRAIN PHYSICS SHAPE");
    // 3.- Calculate inertia.
    btVector3 inertia;
    collisionShape->calculateLocalInertia(0, inertia);
    // 4.- Create BtOgre MotionState (connects Ogre and Bullet).
    BtOgre::RigidBodyState *motionState = new BtOgre::RigidBodyState();
    LogManager::getSingletonPtr()->logMessage("CREATED TERRAIN MOTION STATE");
    // 5.- Create the Body.
    BulletRigidBody *rigidBody = new BulletRigidBody(name, 0, motionState, collisionShape, inertia);
    Ogre::Vector3 pos = tile->getPosition() + Ogre::Vector3(0, (maxH-minH)/2, 0);
    rigidBody->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
    rigidBody->setFriction(3);
    //rigidBody->setHitFraction(30);
    // 6.- Register rigidBody
    mWorld->addRigidBody(rigidBody);
    this->addComponent(rigidBody);
}

void PhysicsLayer::destroyRigidBody(const String &name) {
    BulletRigidBody *rb = static_cast<BulletRigidBody*>(mComponents.findItem(name));
    if (rb) {
        mComponents.removeItem(name);
        mWorld->removeRigidBody(rb);
        delete rb;
    }
}

void PhysicsLayer::setDebugDrawer(RenderLayer *renderLayer) {
    mDebugDrawer = new BtOgre::DebugDrawer(renderLayer->_getSceneManager()->getRootSceneNode(), mWorld);
    mWorld->setDebugDrawer(mDebugDrawer);
    mDebugDrawer->setDebugMode(false);
}

void PhysicsLayer::setDebugMode(bool activate) {
    mDebugDrawer->setDebugMode(activate);
}

bool PhysicsLayer::getDebugMode() {
    return (mDebugDrawer->getDebugMode()!=0);
}

void PhysicsLayer::update(Real deltaTime) {
    mStepTime = deltaTime*mTimeScale;
    mWorld->stepSimulation(mStepTime, 15, btScalar(1.0)/btScalar(120));
    if (mDebugDrawer) mDebugDrawer->step();
}
