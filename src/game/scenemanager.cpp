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

using namespace Caelum;

SceneManager::SceneManager() {
    this->_init(GameEngine::getSingletonPtr());
}

SceneManager::SceneManager(GameEngine *engine) {
    this->_init(engine);
}

void SceneManager::_init(GameEngine *engine) {
    mEngine = engine;
}

SceneManager::~SceneManager() {
    Scene* scene;
    String name;

    while ( !(mScenes.empty()) ) {
        scene = mScenes.getFirstItem();
        name = scene->getName();
        delete scene;
        mScenes.removeItem(name);
    }
}

Scene* SceneManager::createScene(const String &sceneName, const String &typeName) {
    Scene* scene = getScene(sceneName);
    if (!scene) {
        // Create the scene
        scene = new Scene(sceneName, typeName);
        // Add the scene to the dictionary
        mScenes.addItem(sceneName, scene);
    }
    return scene;
}

Scene* SceneManager::getScene(const String &sceneName) {
    return mScenes.findItem(sceneName);
}

void SceneManager::destroyScene(const String sceneName) {
    // delete the scene
    delete getScene(sceneName);
    // remove the item from the associative ocontainer
    mScenes.removeItem(sceneName);
}

void SceneManager::destroyScene(Scene *scene) {
    // delete the scene
    delete scene;
    // remove the item from the dictionary
    mScenes.removeItem(scene);
}
