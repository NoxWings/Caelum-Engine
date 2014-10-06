#include "demos/demo.h"


Demo::Demo(const Caelum::String& sceneName) {
    mSceneName = sceneName;
    mContinue = true;
    mCurrentStage = 0;
}

Demo::~Demo() {
}

void Demo::enter() {
    mContinue = true;
    mCurrentStage = 0;

    // Scene
    mLog->logMessage(Caelum::String("GAMEMANAGER: Creating ")+
                     mSceneName+
                     Caelum::String(" state and scene."));

    mScene = mSceneManager->getScene(mSceneName);
    mSceneOwner = (mScene == 0);
    if (mSceneOwner) {
        mScene = mSceneManager->createScene(mSceneName,"EXTERIOR_CLOSE");
    }
    mRenderLayer = mScene->getRenderLayer();
    mGuiInstance = new Caelum::GuiInstance(mScene);
    mPhysicsLayer = mScene->getPhysicsLayer();
    mAudioLayer = mScene->getAudioLayer();

    if (mSceneOwner) {
        // Camera
        mCamTarget = mScene->createGameObject("CamTarget");
        mCamObj = mCamTarget->createChildGameObject("CamObj");
        mCam = mRenderLayer->createCamera("Cam");
        mCamObj->attachComponent(mCam);
        mCamObj->setFixedYawAxis(true);
    } else {
        mCamTarget = mScene->findObject("CamTarget");
        mCamObj = mScene->findObject("CamObj");
        mCam = static_cast<Caelum::Camera*>(mRenderLayer->getComponentByName("Cam"));
    }
    mCamMouseRotateSpeed = 30;
    mCamJoystickRotateSpeed = 0;
    // Activate
    mCam->setAsActiveCamera();
    mGuiInstance->resume();
}

void Demo::exit() {
    // Deactivate gui scene
    delete mGuiInstance;
    mLog->logMessage(Caelum::String("GAMEMANAGER: Destroying ")+
                     mSceneName+
                     Caelum::String(" state and scene."));
    if (mSceneOwner)
        mSceneManager->destroyScene(mScene);
}

void Demo::pause() {
    mLog->logMessage(Caelum::String("GAMEMANAGER: Pausing ")+
                     mSceneName+
                     Caelum::String(" scene."));
}

void Demo::resume() {
    mLog->logMessage(Caelum::String("GAMEMANAGER: Resuming ")+
                     mSceneName+
                     Caelum::String(" state."));
    mCam->setAsActiveCamera();
    mGuiInstance->resume();
    mAudioLayer->setAsCurrent();
    mCurrentStage = 0;
    setStage(0);
}
