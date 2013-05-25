#ifndef SRC_PHYSICS_PHYSICSMANAGER_H
#define SRC_PHYSICS_PHYSICSMANAGER_H

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "game/layermanager.h"
#include "physics/physicslayer.h"
#include "core/log.h"

namespace Caelum {

class GameEngine;

class PhysicsManager :
        public Singleton<PhysicsManager>,
        public LayerManager {
  public:
    PhysicsManager();
    virtual ~PhysicsManager();

    GameLayer* createLayer(const String& name, const String& typeName);
    PhysicsLayer* createPhysicsLayer(const String& name, const String& typeName);

  private:
    Log* mLog;
    GameEngine* mEngine;
};

}

#endif  // SRC_PHYSICS_PHYSICSMANAGER_H
