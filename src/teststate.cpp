#include "teststate.h"

#include "game/scenemanager.h"
#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreShadowCameraSetup.h>

#include "render/rendermanager.h"

using namespace Caelum;

void TestState::enter() {
    mContinue = true;

    mLog->logMessage("GAMEMANAGER: Creating Test State.");
    mScene = mSceneManager->createScene("TestScene","EXTERIOR_CLOSE");

    /// Camera
    camobj = mScene->createGameObject("CameraObject");
    cam = mScene->getRenderLayer()->createCamera("MainCamera");
    camobj->attachComponent(cam);
    camobj->setPosition(-115,80,300);
    movSpeed = 30;
    rotateSpeed = 3;
    cam->setAsActiveCamera(); // Set active camera

    /// SHADOW CONFIG
    mScene->getRenderLayer()->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));
    mScene->getRenderLayer()->setShadowTechnique(Caelum::SHADOWTYPE_TEXTURE_MODULATIVE);
    mScene->getRenderLayer()->setShadowProjectionType(Caelum::SHADOW_PROJECTION_DEFAULT);
    mScene->getRenderLayer()->setShadowFarDistance(400);
    mScene->getRenderLayer()->setShadowTextureSettings(512, 1); // size of texture shadow + number of shadow buffers

    /// LIGHT
    GameObject *sunObj = mScene->createGameObject("SunObj");
    Light* sunLight = mScene->getRenderLayer()->createLight("SunLight", Light::LT_DIRECTIONAL);
    sunObj->attachComponent(sunLight);
    sunObj->setDirection(-1, -2, -1, GameObject::TS_WORLD);
    // Create Sky
    mScene->getRenderLayer()->setSkyDome(true, "Examples/CloudySky", 10, 8, 400);

    /// SCENE OBJECTS
    // Create House
    GameObject *houseObj = mScene->createGameObject("HouseObject");
    Entity *houseEnt = mScene->getRenderLayer()->createEntity("House", "tudorhouse.mesh");
    houseObj->attachComponent(houseEnt);
    houseObj->setPosition(-110,-10,-50);
    houseObj->scale(0.085,0.085,0.085);
    houseObj->move(0,50,0);

    GameObject *dojoObj = mScene->createGameObject("DojoObject");
    Entity *dojoEnt = mScene->getRenderLayer()->createEntity("Dojo", "ts_dojo.mesh");
    dojoObj->attachComponent(dojoEnt);
    dojoObj->pitch(Degree(-90));
    dojoObj->setPosition(20, 5, 20);
    //camobj->lookAt(dojoObj); // the camera is looking at the dojo


    /// SHADOW TEST
    /*Ogre::SceneManager* sceneMgr = mScene->getSceneMgr();
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground",
                                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            plane,1500,1500,200,200,true,1,5,5,Ogre::Vector3::UNIT_Z);
    // create anentity with that plane
    Ogre::Entity *ent = sceneMgr->createEntity("GroundEntity", "ground");
    ent->setMaterialName("Examples/GrassFloor");
    // deactivate its shadows
    ent->setCastShadows(false);
    sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);*/
}

void TestState::exit() {
    mLog->logMessage("GAMEMANAGER: Exiting Test State");
    mSceneManager->destroyScene(mScene);
}

void TestState::pause() {
}

void TestState::resume() {
    cam->setAsActiveCamera();
}

bool TestState::preRenderUpdate(const Caelum::RenderEvent &evt) {
    // Movement
    mov.normalise(); // normalise the movement (uniform movement)
    mov *= (movSpeed * evt.timeSinceLastRender);
    camobj->move(mov.x, 0, mov.z, GameObject::TS_LOCAL);
    camobj->move(0, mov.y, 0, GameObject::TS_WORLD);
    // Rotation
    camobj->yaw(Degree(rotateSpeed * yawRot * evt.timeSinceLastRender), GameObject::TS_WORLD);
    camobj->pitch(Degree(rotateSpeed * pitchRot * evt.timeSinceLastRender), GameObject::TS_LOCAL);
    yawRot = 0;
    pitchRot = 0;
    return mContinue;
}

bool TestState::renderingUpdate(const Caelum::RenderEvent &evt) { return mContinue;}
bool TestState::postRenderUpdate(const Caelum::RenderEvent &evt) { return mContinue;}

bool TestState::mouseMoved(const Caelum::MouseEvent& evt) {
    yawRot = -evt.X.rel;
    pitchRot = -evt.Y.rel;
    return true;
}

bool TestState::mousePressed(const Caelum::MouseEvent& evt, Caelum::MouseButtonID id) {
    return true;
}

bool TestState::mouseReleased(const Caelum::MouseEvent& evt, Caelum::MouseButtonID id) {
    return true;
}

bool TestState::mouseClicked(const Caelum::MouseEvent& evt, Caelum::MouseButtonID id) {
    return true;
}

bool TestState::keyPressed(const Caelum::KeyEvent &evt) {
    if (evt.key == KC_ESCAPE) { mContinue = false; }
    else if (evt.key == KC_W) { mov.z = -1;}
    else if (evt.key == KC_S) { mov.z = 1;}
    else if (evt.key == KC_A) { mov.x = -1;}
    else if (evt.key == KC_D) { mov.x = 1;}
    else if (evt.key == KC_SPACE) { mov.y = 1;}
    else if (evt.key == KC_LCONTROL) { mov.y =-1;}
    else if (evt.key == KC_LSHIFT) { movSpeed = 60;}
    return true;
}

bool TestState::keyReleased (const Caelum::KeyEvent &evt) {
    if (evt.key == KC_W) { mov.z = 0;}
    else if (evt.key == KC_S) { mov.z = 0;}
    else if (evt.key == KC_A) { mov.x = 0;}
    else if (evt.key == KC_D) { mov.x = 0;}
    else if (evt.key == KC_SPACE) { mov.y = 0;}
    else if (evt.key == KC_LCONTROL) { mov.y = 0;}
    else if (evt.key == KC_LSHIFT) { movSpeed = 30;}
    return true;
}

bool TestState::keyTap(const Caelum::KeyEvent &evt) {
    return true;
}
