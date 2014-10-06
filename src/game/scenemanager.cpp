/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "game/scenemanager.h"

#include "core/gameengine.h"
#include "core/logmanager.h"


using namespace Caelum;

SceneManager::SceneManager() {
    this->_init(GameEngine::getSingletonPtr());
}

SceneManager::SceneManager(GameEngine *engine) {
    this->_init(engine);
}

void SceneManager::_init(GameEngine *engine) {
    mEngine = engine;
    mLog = mEngine->getLogManager()->getLog("GameManager.log");
    mLog->logMessage("GAMEMANAGER: Creating Scene Manager");
}

SceneManager::~SceneManager() {
    mLog->logMessage("GAMEMANAGER: Destroying Scene Manager");
    Scene* scene;
    String name;

    while ( !(mScenes.empty()) ) {
        scene = mScenes.getFirstItem();
        name = scene->getName();
        mLog->logMessage("GAMEMANAGER: Destroying Scene: "+name);
        mScenes.removeItem(name);
        mLog->logMessage("GAMEMANAGER: Removed Scene: "+name);
        delete scene;
        mLog->logMessage("GAMEMANAGER: Destroyed Scene: "+name);
    }
    mLog->logMessage("GAMEMANAGER: All scenes are destroyed.");
}

Scene* SceneManager::createScene(const String &sceneName, const String &typeName) {
    Scene* scene = getScene(sceneName);
    if (!scene) {
        // Create the scene
        scene = new Scene(sceneName, typeName);
        // Add the scene to the dictionary
        mScenes.addItem(sceneName, scene);
        mLog->logMessage("GAMEMANAGER: New Scene Created: "+sceneName);
    }
    return scene;
}

Scene* SceneManager::getScene(const String &sceneName) {
    return mScenes.findItem(sceneName);
}

void SceneManager::destroyScene(const String sceneName) {
    Scene *temp = getScene(sceneName);
    // remove the item from the associative ocontainer
    mScenes.removeItem(sceneName);
    // delete the scene
    delete temp;
}

void SceneManager::destroyScene(Scene *scene) {
    // remove the item from the dictionary
    mScenes.removeItem(scene);
    // delete the scene
    delete scene;
}
