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
    // Sound layer
    // Physics layer
    mPhysicsLayer = PhysicsManager::getSingletonPtr()->createPhysicsLayer(sceneName, typeName);
    mPhysicsLayer->setDebugDrawer(mRenderLayer);
    // Net layer

    // Create the root object
    _mScene = mRenderLayer->_getSceneManager(); // Get the render scene from the Layer
    mRootObject = new GameObject(_mScene->getRootSceneNode()); // Root game object
}

Scene::~Scene() {
    // Delete on cascade the root object and its children
    delete mRootObject;
    _mScene = 0;

    /// Delete layers
    // Net layer
    // Physics layer
    PhysicsManager::getSingletonPtr()->destroyLayer(mPhysicsLayer);
    // Sound layer
    RenderManager::getSingletonPtr()->destroyLayer(mRenderLayer);
}

GameObject* Scene::createGameObject(const String &name) {
    return mRootObject->createChildGameObject(name);
}
