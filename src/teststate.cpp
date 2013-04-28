#include "teststate.h"

#include "game/scenemanager.h"
#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreShadowCameraSetup.h>

#include "render/rendermanager.h"

using namespace Caelum;

void TestState::enter() {
    mLog->logMessage("GAMEMANAGER: Creating Test State.");
    mScene = mSceneManager->createScene("TestScene","EXTERIOR_CLOSE");

    // Create Camera
    GameObject *camobj = mScene->createGameObject("CameraObject");
    CameraComponent *cam = mScene->getRenderLayer()->createCamera("MainCamera");
    camobj->attachComponent(cam);

    // Create House
    GameObject *dojoObj = mScene->createGameObject("DojoObject");
    Entity *ent = mScene->getRenderLayer()->createEntity("Dojo", "tudorhouse.mesh");
    dojoObj->attachComponent(ent);
    dojoObj->setPosition(-50,0,-50);
    dojoObj->scale(0.085,0.085,0.085);
    dojoObj->move(0,46.5,0);

    // Create Dome
    mScene->getRenderLayer()->setSkyDome(true, "Examples/CloudySky", 10, 8, 400);

    //Ogre::SceneManager* mSceneMgr = mScene->getSceneMgr();
    //Ogre::Camera* mCamera = mSceneMgr->createCamera("MainCamera");
    // Clip Distances
    //mCamera->setFarClipDistance(300000);
    //mCamera->setNearClipDistance(0.25);
    // Position & Orientation
    //mCamera->setPosition(Ogre::Vector3(0,200,500));
    //mCamera->lookAt(Ogre::Vector3(0, 300, -1));
    //mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE); // Technique & lightning
    //mSceneMgr->setShadowTextureSettings(256,1); // size of texture shadow + number of shadow buffers
    //mSceneMgr->setShadowFarDistance(400); // Far distance
    // Shadow Projection
    //Ogre::LiSPSMShadowCameraSetup *mLiSPSMSetup = new Ogre::LiSPSMShadowCameraSetup();
    //mSceneMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(mLiSPSMSetup));
    // Viewport
    //RenderManager::getSingletonPtr()->getRenderWindow()->getActualWindow()->addViewport(mCamera);
    //mCamera->setAspectRatio(Real(1024)/Real(768));

    /// AMBIENT STUFF
    /*Ogre::Entity* stuff = mSceneMgr->createEntity("house", "tudorhouse.mesh");
    Ogre::SceneNode* stuffnode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-50, 0, -50));
    stuffnode->attachObject(stuff);
    stuffnode->scale(0.085, 0.085, 0.085);
    stuffnode->translate(0,46.5,0);*/

    //mSceneMgr->setSkyDome(true, "Examples/CloudySky",10,8,400);
    /// SUN LIGHT
    //mSceneMgr->createLight("SunLight")->setType(Ogre::Light::LT_DIRECTIONAL);

    // Set active camera
    cam->setAsActiveCamera();
}

void TestState::exit() {
    mLog->logMessage("GAMEMANAGER: Exiting Test State");
    mSceneManager->destroyScene(mScene);
}

void TestState::pause() {
}

void TestState::resume() {
}

bool TestState::preRenderUpdate(const Caelum::RenderEvent &evt) {
    return true;
}

bool TestState::renderingUpdate(const Caelum::RenderEvent &evt) {
    return true;
}

bool TestState::postRenderUpdate(const Caelum::RenderEvent &evt) {
    return true;
}
