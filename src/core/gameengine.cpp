/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "core/gameengine.h"

#include <OGRE/Ogre.h>
#include "core/ogre/ogrelogmanager.h"
#include "render/ogre/ogrerendermanager.h"
#include "input/oisinputmanager.h"

using namespace Caelum;

GameEngine::GameEngine()
    : mLogManager(0),
      mPluginMan(0),
      mResourceMan(0),
      mRenderMan(0) {
    createLogManager();
    createMainSystem();
    createPluginManager();
}

GameEngine::~GameEngine() {
    // try to shutdown the subsystems if the user hasn't done so
    shutdown();
    destroyPluginManager();
    destroyMainSystem();
    /* The main system (ogre) will try to destroy the log but it uses smart pointers
     * so as long as we dont delete the logmanager explicitly it wont deallocate */
    destroyLogManager();
}


void GameEngine::setup() {
    createResourceManager();
    createPreferenceManager();
    createRenderManager();
    createInputManager();
    createPhysicsManager();

    createGameManager();
    // TODO remove the following line
    mResourceMan->initialiseAllResources();
}

void GameEngine::shutdown() {
    destroyGameManager();

    destroyPhysicsManager();
    destroyInputManager();
    destroyRenderManager();
    destroyPreferenceManager();
    destroyResourceManager();
}

/// **************************************
/// CREACIÓN
/// **************************************

void GameEngine::createLogManager() {
    mLogManager = new OgreLogManager();
}

void GameEngine::createPluginManager() {
    mPluginMan = new PluginManager();
    mPluginMan->loadPluginsFromFile("../config/plugins.cfg");
}

void GameEngine::createMainSystem() {
    // Create Ogre without loading any default config nor plugins
    new Ogre::Root("","","");
    mLogManager->createDefaultLog("caelum.log");
    mLogManager->logMessage("******************************");
    mLogManager->logMessage("*** Starting CAELUM-ENGINE ***");
    mLogManager->logMessage("******************************");
}

void GameEngine::createResourceManager() {
    mResourceMan = new ResourceManager("../config/resources.cfg");
}

void GameEngine::createPreferenceManager() {
    mPreferenceMan = new PreferenceManager("../config/caelum.cfg");
}

void GameEngine::createRenderManager() {
    mRenderMan = new OgreRenderManager();
}

void GameEngine::createInputManager() {
    // Create and register Input Manager
    mInputMan = new OISInputManager();
}

void GameEngine::createPhysicsManager() {
    mPhyManager = new PhysicsManager();
}

void GameEngine::createGameManager() {
    mGameMan = new GameManager(this);
}

/// **************************************
/// DESTRUCTION
/// **************************************

void GameEngine::destroyLogManager() {
    delete mLogManager;
    mLogManager = 0;
}

void GameEngine::destroyPluginManager() {
    delete mPluginMan;
    mPluginMan = 0;
}

void GameEngine::destroyMainSystem() {
    mLogManager->logMessage("******************************");
    mLogManager->logMessage("*** Stopping CAELUM-ENGINE ***");
    mLogManager->logMessage("******************************");
    delete Ogre::Root::getSingletonPtr();
}

void GameEngine::destroyResourceManager() {
    delete mResourceMan;
    mResourceMan = 0;
}

void GameEngine::destroyPreferenceManager() {
    delete mPreferenceMan;
    mPreferenceMan = 0;
}

void GameEngine::destroyRenderManager() {
    delete mRenderMan;
    mRenderMan = 0;
}

void GameEngine::destroyInputManager() {
    delete mInputMan;
    mInputMan = 0;
}

void GameEngine::destroyPhysicsManager() {
    delete mPhyManager;
    mPhyManager = 0;
}

void GameEngine::destroyGameManager() {
    delete mGameMan;
    mGameMan = 0;
}
