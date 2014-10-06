#include "physics/physicsmanager.h"

#include "core/gameengine.h"
#include "core/logmanager.h"

using namespace Caelum;

PhysicsManager::PhysicsManager() {
    mEngine = GameEngine::getSingletonPtr();
    mLog = mEngine->getLogManager()->createLog("physics.log");
    mLog->logMessage("*PHYSICS: Physics Engine Init");
}

PhysicsManager::~PhysicsManager() {
    destroyAllLayers();
    mLog->logMessage("*PHYSICS: Physics Engine Shutting down");
    mEngine->getLogManager()->destroyLog(mLog);
    mLog = 0;
    mEngine = 0;
}

GameLayer* PhysicsManager::createLayer(const String &name, const String &typeName) {
    return createPhysicsLayer(name, typeName);
}

PhysicsLayer* PhysicsManager::createPhysicsLayer(const String &name, const String &typeName) {
    PhysicsLayer* layer = new PhysicsLayer(name, typeName);
    this->addLayer(layer);
    return layer;
}
