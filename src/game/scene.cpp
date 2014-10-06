/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "scene.h"

#include "core/gameengine.h"
#include <OGRE/OgreSceneManager.h>

using namespace Caelum;

Scene::Scene(const String &sceneName, const String& typeName) {
    mName = sceneName;
    mTypeName = typeName;

    mLog = LogManager::getSingletonPtr()->getLog("GameManager.log");

    /// Create the default layers
    // Render Layer
    mRenderLayer = RenderManager::getSingletonPtr()->createRenderLayer(sceneName, typeName);
    // Physics layer
    mPhysicsLayer = PhysicsManager::getSingletonPtr()->createPhysicsLayer(sceneName, typeName);
    mPhysicsLayer->setDebugDrawer(mRenderLayer);
    // Audio layer
    mAudioLayer = AudioManager::getSingletonPtr()->createAudioLayer(sceneName, typeName);
    // Net layer

    // Create the root object
    _mScene = mRenderLayer->_getSceneManager(); // Get the render scene from the Layer
    mRootObject = new GameObject(this, _mScene->getRootSceneNode()); // Root game object
}

Scene::~Scene() {
    mObjectMap.clear();
    // Delete on cascade the root object and its children
    delete mRootObject;
    _mScene = 0;

    /// Delete layers
    // Net layer
    // Audio layer
    AudioManager::getSingletonPtr()->destroyLayer(mAudioLayer);
    // Physics layer
    PhysicsManager::getSingletonPtr()->destroyLayer(mPhysicsLayer);
    mLog->logMessage(mName+String(": *Physics layer destroyed"));
    // Render layer
    RenderManager::getSingletonPtr()->destroyLayer(mRenderLayer);
    mLog->logMessage(mName+String(": *Render layer destroyed"));
}

GameObject *Scene::findObject(const String &name) {
    std::map<String, GameObject*>::iterator it = mObjectMap.find(name);
    if (it != mObjectMap.end())
        return it->second;
    else
        return 0;
}

GameObject* Scene::createGameObject(const String &name) {
    return mRootObject->createChildGameObject(name);
}


void Scene::notifyCreated(GameObject *obj) {
    mObjectMap[obj->getName()] = obj;
}

void Scene::notifyDeleted(GameObject *obj) {
    mObjectMap.erase(obj->getName());
}
