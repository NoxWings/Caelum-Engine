/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "render/rendermanager.h"

#include <OGRE/Ogre.h>
#include "core/gameengine.h"
#include "render/ogre/ogrerenderwindow.h"
#include "render/guimanager.h"

using namespace Caelum;


RenderManager::RenderManager()
    : mIsRenderLoopActive(false) {
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
    // Create the render window with its asociated event dispatcher
    createRenderWindow();
}

RenderManager::~RenderManager() {
    mLog->logMessage("*RENDER: Rendering Engine Shutting down");

    // Destroy the render window
    destroyRenderWindow();

    // Shutdown the render system
    Ogre::Root::getSingletonPtr()->shutdown();

    // unregister plugin loader
    mEngine->getPluginManager()->unregisterPluginLoader(this, false);

    mLog->logMessage("*RENDER: Rendering Engine Completely Shutdown!");
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

void RenderManager::addRenderListener(RenderListener *listener) {
    this->addItem(listener);
}

void RenderManager::removeRenderListener(RenderListener *listener) {
    this->removeItem(listener);
}

void RenderManager::addWindowEventListener(WindowListener *listener) {
    mWindow->addListener(listener);
}

void RenderManager::removeWindowEventListener(WindowListener *listener) {
    mWindow->removeListener(listener);
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

GameLayer* RenderManager::createLayer(const String &name, const String &typeName) {
    GameLayer *layer = new RenderLayer(name, typeName); // create a render layer
    // help function from LayerManager
    this->addLayer(layer);
    return layer;
}

RenderLayer* RenderManager::createRenderLayer(const String &name, const String &typeName) {
    return static_cast<RenderLayer*>(this->createLayer(name, typeName));
}

void RenderManager::createRenderWindow() {
    mLog->logMessage("*RENDER: Creating Render Window");
    PreferenceManager* pm = mEngine->getPreferenceManager();
    mWindow = new OgreRenderWindow(pm->getWindowName(),
                                   pm->getResolutionWidth(),
                                   pm->getResolutionHeight(),
                                   pm->getFullScreen(),
                                   pm->getVideoOptions());
}

void RenderManager::destroyRenderWindow() {
    mLog->logMessage("*RENDER: Destroying Render Window");
    delete mWindow;
}
