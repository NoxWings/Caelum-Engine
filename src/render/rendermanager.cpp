#include "render/rendermanager.h"

#include <OGRE/Ogre.h>
#include "core/gameengine.h"

using namespace Caelum;


RenderManager::RenderManager() {
    mEngine = GameEngine::getSingletonPtr();
    mLog = mEngine->getLogManager()->createLog("Rendering.log");
    mLog->logMessage("*RENDER: Rendering Engine Init");
}

RenderManager::~RenderManager() {
    mLog->logMessage("*RENDER: Rendering Engine Shutdown");
    mEngine->getLogManager()->destroyLog(mLog);
    mLog = 0;
    mEngine = 0;
}

void RenderManager::loadPlugin(const String &pluginPath) {
    Ogre::Root::getSingletonPtr()->loadPlugin(pluginPath);
    mLog->logMessage("Plugin loaded \'"+pluginPath+"\'");
}

void RenderManager::unloadPlugin(const String &pluginPath) {
    Ogre::Root::getSingletonPtr()->unloadPlugin(pluginPath);
    mLog->logMessage("Plugin Unloaded \""+pluginPath+"\"");
}

bool RenderManager::isPluginLoaded(const String &pluginPath) {
    // TODO
    return false;
}
