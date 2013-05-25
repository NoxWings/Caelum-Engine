#ifndef PHYSICSLAYER_H
#define PHYSICSLAYER_H

#include "EnginePrerequisites.h"

#include "game/gamelayer.h"
#include "render/renderlayer.h"
#include "physics/rigidbody.h"


/// Forward declarations
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDynamicsWorld;
namespace BtOgre {
class DebugDrawer;
}


namespace Caelum {

class PhysicsLayer : public GameLayer {
  public:
    enum PHY_OBJECT_TYPE {
        PHY_OBJECT_STATIC,
        PHY_OBJECT_DINAMIC,
        PHY_OBJECT_TYPES
    };

    enum PHY_SHAPE_TYPE {
        PHY_SHAPE_BOX,
        PHY_SHAPE_CONVEX,
        PHY_SHAPE_CYLINDER,
        PHY_SHAPE_SPHERE,
        PHY_SHAPE_TRIMESH,
        PHY_SHAPE_TYPES
    };

  public:
    PhysicsLayer(const String& name, const String& typeName);
    virtual ~PhysicsLayer();

    /// Component creation
    RigidBody* createRigidBody(const String& name, Entity *ent, const Vector3& scale, Real mass, PHY_SHAPE_TYPE shapeType);
    void createStaticTerrain(Terrain *terrain);

    /// Getters setters
    void setDebugDrawer(RenderLayer *renderLayer);
    void setDebugMode(bool activate);
    bool getDebugMode();

    /// Game layer functions
    void update(Real deltaTime);

  private:
    btBroadphaseInterface *mBroadphase;
    btDefaultCollisionConfiguration *mCollisionConfig;
    btCollisionDispatcher *mDispatcher;
    btSequentialImpulseConstraintSolver *mSolver;

    btDynamicsWorld* mWorld;
    BtOgre::DebugDrawer* mDebugDrawer;
    Real mTimeScale, mStepTime;
};

}

#endif // PHYSICSLAYER_H
