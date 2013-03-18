#include "render/rendermanager.h"

#include <OGRE/Ogre.h>
#include "core/gameengine.h"

using namespace Caelum;


RenderManager::RenderManager() {
    mEngine = GameEngine::getSingletonPtr();
    // Create Own Log Manager
    mLog = mEngine->getLogManager()->createLog("rendering.log");
    mLog->logMessage("*RENDER: Rendering Engine Init");

    // Register as a plugin loader, and get at queued plugins for us
    mLog->logMessage("Loagind render plugins:");
    mEngine->getPluginManager()->registerPluginLoader("Render", this);

    // Initialise the render system
    PreferenceManager* pm = mEngine->getPreferenceManager();
    mLog->logMessage("*Setting render system: '"+pm->getRenderSystem()+"'");
    if ( !setRenderSystem(pm->getRenderSystem()) ) {
        mLog->logMessage("ERROR: The selected render system could not be loaded!!");
    }
    createRenderWindow();
}

RenderManager::~RenderManager() {
    mLog->logMessage("*RENDER: Rendering Engine Shutdown");
    destroyRenderWindow();

    // Shutdown the render system
    Ogre::Root::getSingletonPtr()->shutdown();

    // unregister plugin loader
    mEngine->getPluginManager()->unregisterPluginLoader(this);

    // Destroy the rendering log
    mEngine->getLogManager()->destroyLog(mLog);
    mLog = 0;
    mEngine = 0;
}

bool RenderManager::setRenderSystem(const String &renderSystem) {
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    /* Ogre render system names are
     * OpenGL = "OpenGL Rendering Subsystem"
     * Direct3D9 = "Direct3D9 Rendering Subsystem"
     * Direct3D10 = "Direct3D11 Rendering Subsystem"
     */
    Ogre::RenderSystem *rs = root->getRenderSystemByName(renderSystem + " Rendering Subsystem");
    if (rs == 0) { return false;}
    root->setRenderSystem(rs);
    // Initialise the rendering engine
    root->initialise(false);
    return true;
}

void RenderManager::registerWindowEventListener() {
    Ogre::WindowEventUtilities::addWindowEventListener();
}

void RenderManager::unregisterWindowEventListener() {
    Ogre::WindowEventUtilities::removeWindowEventListener();
}

void RenderManager::loadPlugin(const String &pluginPath) {
    Ogre::Root::getSingletonPtr()->loadPlugin(pluginPath);
    registerSelfPlugin(pluginPath);
    mLog->logMessage("Plugin loaded \'"+pluginPath+"\'");
}

void RenderManager::unloadPlugin(const String &pluginPath) {
    Ogre::Root::getSingletonPtr()->unloadPlugin(pluginPath);
    unregisterSelfPlugin(pluginPath);
    mLog->logMessage("Plugin Unloaded \""+pluginPath+"\"");
}

void RenderManager::createRenderWindow() {
    mLog->logMessage("*RENDER: Creating Render Window");
    Ogre::Root *root = Ogre::Root::getSingletonPtr();

    PreferenceManager* pm = mEngine->getPreferenceManager();
    PreferenceManager::VideoOptions *vo = pm->getVideoOptions();
    Ogre::NameValuePairList nvp;
    for (PreferenceManager::VideoOptions::iterator it = vo->begin();
         it != vo->end();
         ++it) {
        nvp.insert(Ogre::NameValuePairList::value_type(it->first, it->second));
    }

    Ogre::RenderWindow* rw = root->createRenderWindow( pm->getWindowName(),
                                                       pm->getResolutionWidth(),
                                                       pm->getResolutionHeight(),
                                                       pm->getFullScreen(),
                                                       &nvp );
}

void RenderManager::destroyRenderWindow() {
    mLog->logMessage("*RENDER: Destroying Render Window");
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    root->destroyRenderTarget(mEngine->getPreferenceManager()->getWindowName());
}
