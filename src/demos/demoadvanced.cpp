#include "demoadvanced.h"

#include "render/rendermanager.h"

#include <OGRE/OgreLight.h>

using namespace Caelum;

DemoAdvanced::DemoAdvanced(const Caelum::String &sceneName)
    : Demo(sceneName) {
}

DemoAdvanced::~DemoAdvanced() {
}

void DemoAdvanced::setStage(int n) {
    switch (n) {
    case 0:
        mRenderLayer->setBackgroundColour(Caelum::ColourValue::White);
        replaceLayout("Advanced_extension");
        break;
    case 1:
        replaceLayout("Advanced_optimizaciones");
        break;
    case 2:
        replaceLayout("Advanced_materiales");
        break;
    case 3:
        replaceLayout("Advanced_shaders");
        setupActualScene();
        activateMusic();
        break;
    case 4:
        replaceLayout("Advanced_shaders2");
        mCamObj->setPosition(1000, 300, 2000);
        mCamObj->yaw(Degree(180));
        //createTerrain();
        break;
    case 5:
        replaceLayout("Advanced_shaders2_hide");
        break;
    default:
        if (mCurrentStage < 0) {
            // previous demo
            mGameMan->transitionBack();
        } else {
            // next demo
            mGameMan->transitionBack();
        }
        break;
    }
}

void DemoAdvanced::replaceLayout(const String &layout) {
    if (mCurrentLayout.size() > 1) {
        mGuiInstance->unloadLayout(mCurrentLayout);
    }
    mCurrentLayout = layout;
    mGuiInstance->loadLayout(mCurrentLayout, "");
}

void DemoAdvanced::setCollisionType(PhysicsLayer::PHY_SHAPE_TYPE type) {
    if (type != mCollisionType) {
        //clearObjects();
        mCollisionType = type;
    }
    mLog->logMessage("Collision type completely set!");
}

void DemoAdvanced::clearObjects(){
    while (barrelAdvancedCount > 0) {
        static String counter;          // string which will contain the result
        std::ostringstream convert;   // stream used for the conversion
        convert << barrelAdvancedCount-1;      // insert the textual representation of 'Number' in the characters in the stream
        counter = convert.str(); // set 'Result' to the contents of the stream

        // Object creation
        mPhysicsLayer->destroyRigidBody(String("barrelphy")+counter);
        mScene->getRootObject()->destroyChildGameObject(String("barrelobj")+counter);
        mRenderLayer->destroyComponent(String("barrel")+counter);
        barrelAdvancedCount--;
    }
    barrelAdvancedCount = 0;
}

void DemoAdvanced::launchObject(Caelum::GameObject *ref, Real force) {
    static String count;          // string which will contain the result
    std::ostringstream convert;   // stream used for the conversion
    convert << barrelAdvancedCount++;      // insert the textual representation of 'Number' in the characters in the stream
    count = convert.str(); // set 'Result' to the contents of the stream

    // Object creation
    GameObject *barrelObj = mScene->createGameObject(String("barrelobj")+count);
    Entity *barrelEnt;
    switch (mCollisionType) {
    case PhysicsLayer::PHY_SHAPE_BOX:
        barrelEnt = mRenderLayer->createEntity(String("barrel")+count, "cube.mesh");
        break;
    case PhysicsLayer::PHY_SHAPE_SPHERE:
        barrelEnt = mRenderLayer->createEntity(String("barrel")+count, "sphere.mesh");
        break;
    case PhysicsLayer::PHY_SHAPE_CONVEX:
        barrelEnt = mRenderLayer->createEntity(String("barrel")+count, "Barrel.mesh");
        break;
    case PhysicsLayer::PHY_SHAPE_CYLINDER:
    default:
        barrelEnt = mRenderLayer->createEntity(String("barrel")+count, "Barrel.mesh");
        break;
    }
    barrelObj->attachComponent(barrelEnt);

    RigidBody *barrelPhy;
    switch (mCollisionType) {
    case PhysicsLayer::PHY_SHAPE_BOX:
    case PhysicsLayer::PHY_SHAPE_SPHERE:
        barrelObj->setScale(Vector3::UNIT_SCALE/20);
        barrelObj->setScale(Vector3::UNIT_SCALE/20);
        barrelObj->notifyScale();
        barrelPhy = mScene->getPhysicsLayer()->createRigidBody(String("barrelphy")+count, barrelEnt, Vector3::UNIT_SCALE/20, 1, mCollisionType);
        barrelObj->attachComponent(barrelPhy);
        break;
    case PhysicsLayer::PHY_SHAPE_CYLINDER:
    case PhysicsLayer::PHY_SHAPE_CONVEX:
    default:
        barrelPhy = mScene->getPhysicsLayer()->createRigidBody(String("barrelphy")+count, barrelEnt, Vector3::UNIT_SCALE, 1, PhysicsLayer::PHY_SHAPE_CONVEX);
        barrelObj->attachComponent(barrelPhy);
        break;
    }
    barrelObj->setDerivedPosition(ref->getDerivedPosition());
    barrelObj->setDerivedOrientation(ref->getDerivedOrientation());
    barrelObj->move(-(barrelObj->getOrientation().zAxis().normalisedCopy())*3);
    barrelObj->pitch(Degree((60*mLaunchPitch)+5));
    barrelObj->yaw(Degree(60*mLaunchRoll));
    barrelObj->notifyTransform();
    // Launch
    Real impulseforce = force;
    barrelPhy->applyImpulse(-(barrelObj->getDerivedOrientation().zAxis().normalisedCopy())*impulseforce);
}

void DemoAdvanced::activateMusic() {
    mAudioLayer->setVolume(1.0f);
    if (!mMusicSource) {
        mMusicSource = mAudioLayer->createAudioSource("BackgroundSong2", "merchantprince.ogg");
        mScene->getRootObject()->attachComponent(mMusicSource);
        mMusicSource->setLoop(true);
    }
    if (!mMusicSource->isplaying()) {
        mMusicSource->play();
    }
}

void DemoAdvanced::setupActualScene() {
    mSky = static_cast<RealisticSky*>(mRenderLayer->getComponentByName("DemoMainSky"));
    mOcean = static_cast<OceanSimulator*>(mRenderLayer->getComponentByName("DemoMainOcean"));
    /*mAudioLayer->setVolume(0.2f);
    mCamObj->attachComponent(mAudioLayer->getAudioReceptor());
    mMusicSource = */
}

void DemoAdvanced::createTerrain() {
    mOcean->setPosition(Vector3(0, 120, 0));
    mTerrain = mRenderLayer->createTerrain("Island", 513, 4000);
    mTerrain->configureImport(500, 6, 8, 4000, 10);
    mTerrain->loadTile(0, 0, "Island.dat");
    mTerrain->loadAllTiles();
}

void DemoAdvanced::enter() {
    Demo::enter();
    mCurrentLayout = "";
    mCamMovSpeed = 400;
    mCamJoystickRotateSpeed = 150;
    mCamPitch = mCamYaw = 0;
    mCamAxisPitch = mCamAxisYaw = 0;
    mLaunchPitch = mLaunchRoll = 0;
    mCamMov = Vector3::ZERO;
    mCollisionType = Caelum::PhysicsLayer::PHY_SHAPE_CONVEX;
    barrelAdvancedCount = 0;
    mSkyPreset = RealisticSky::SKY_DESERT;
    mMusicSource = 0;

    /*GameObject *houseObj = mScene->createGameObject("HouseObject");
    Entity *houseEnt = mScene->getRenderLayer()->createEntity("House", "tudorhouse.mesh");
    houseEnt->setShadowCast(true);
    RigidBody *housePhy = mScene->getPhysicsLayer()->createRigidBody("HousePhy", houseEnt, Vector3::UNIT_SCALE, 0, PhysicsLayer::PHY_SHAPE_TRIMESH);
    houseObj->attachComponent(houseEnt);
    houseObj->attachComponent(housePhy);
    houseObj->setPosition(-110,0,-50);
    houseObj->scale(0.085,0.085,0.085);
    houseObj->move(0,340,0);
    houseObj->notifyTransform();*/


    //mCamObj->setPosition(100,0,100);
    //mCamObj->lookAt(mPlatformObj);
    resume();
}

void DemoAdvanced::exit() {
    mMusicSource->stop();
    clearObjects();
    Demo::exit();
}

void DemoAdvanced::pause() {
    mGuiInstance->pause();
    Demo::pause();
}

void DemoAdvanced::resume() {
    Demo::resume();
    mGuiInstance->resume();
    mGuiInstance->setPointerVisible(false);
}

void DemoAdvanced::preRender(const Caelum::RenderEvent &evt) {
    //mCamMov.normalise();
    Vector3 mov = mCamMov *(mCamMovSpeed * evt.timeSinceLastRender);
    mCamObj->move(mov.x, 0, mov.z, Caelum::GameObject::TS_LOCAL);
    mCamObj->move(0, mov.y, 0, Caelum::GameObject::TS_WORLD);

    mCamYaw = Caelum::Degree(mCamAxisYaw*mCamJoystickRotateSpeed*evt.timeSinceLastRender);
    mCamPitch = Caelum::Degree(mCamAxisPitch*mCamJoystickRotateSpeed*evt.timeSinceLastRender);
    mCamObj->yaw(mCamYaw, Caelum::GameObject::TS_WORLD);
    mCamObj->pitch(mCamPitch, Caelum::GameObject::TS_LOCAL);
    mCamObj->forceUpdate();
}

void DemoAdvanced::rendering(const Caelum::RenderEvent &evt) {
}

void DemoAdvanced::postRender(const Caelum::RenderEvent &evt) {
}

bool DemoAdvanced::mouseMoved(const MouseEvent &evt) {
    return true;
}

bool DemoAdvanced::mousePressed(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoAdvanced::mouseReleased(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoAdvanced::keyPressed(const KeyEvent &evt) {
    return true;
}

bool DemoAdvanced::keyReleased(const KeyEvent &evt) {
    if (evt.key == Caelum::KC_ESCAPE) {
        mContinue = false;
    }
    return true;
}

bool DemoAdvanced::buttonPressed(const Caelum::JoyStickEvent &arg, int button) {
    if (button == 4) { // L1 previous
        previousStage();
    } else if (button == 5) { // R1 next
        nextStage();
    } else if (button == 1) { // R2 debug physics
        bool debug = mPhysicsLayer->getDebugMode();
        mPhysicsLayer->setDebugMode(!debug);
    } else if (button == 3) {
        POLYGON_MODE newMode;
        switch (mCam->getPolygonMode()) {
        case PM_WIREFRAME:
            newMode = PM_SOLID;
            break;
        case PM_SOLID:
            newMode = PM_WIREFRAME;
            break;
        default:
            newMode = PM_SOLID;
        }
        mCam->setPolygonMode( newMode );
    } else if (button == 2) {
        launchObject(mCamObj, 60);
    } else if (button == 0) {
        launchObject(mCamObj, 0);
    } else if (button == 6) {
        Real volume = mAudioLayer->getVolume();
        volume -= 0.1;
        if (volume < 0) volume = 0;
        mAudioLayer->setVolume(volume);
    } else if (button == 7) {
        Real volume = mAudioLayer->getVolume();
        volume += 0.1;
        if (volume > 1) volume = 1;
        mAudioLayer->setVolume(volume);
    }
}

bool DemoAdvanced::buttonReleased(const Caelum::JoyStickEvent &arg, int button) {
}

bool DemoAdvanced::axisMoved(const Caelum::JoyStickEvent &arg, int axis) {
    // Calculate percentage
    int moved = arg.mState.mAxes[axis].abs;
    Real percentage = Caelum::Real(moved) / (Caelum::Real(Caelum::MAX_AXIS));
    if (percentage < 0.1f && percentage > -0.1f) percentage = 0.0f;
    if (percentage < -1.0f) percentage = -1.0f;

    if (axis == 2) { // pitch
        mCamAxisPitch = -percentage;
    } else if (axis == 5) { // yaw
        mCamAxisYaw = -percentage;
    } else if (axis == 0) {
        mCamMov.z = percentage;
    } else if (axis == 1) {
        mCamMov.x = percentage;
    } else if (axis == 3) { // launch roll
        mLaunchRoll = percentage;
        if (mLaunchRoll > 0.5) mLog->logMessage("Rolling!!");
    } else if (axis == 4) { // launch pitch
        mLaunchPitch = percentage;
    }
    return true;
}

bool DemoAdvanced::sliderMoved(const Caelum::JoyStickEvent &arg, int index) {

}

bool DemoAdvanced::povMoved(const Caelum::JoyStickEvent &arg, int index) {
    switch (arg.mState.mPOV[index].direction) {
    case Pov::North:

        break;
    case Pov::South: {
        clearObjects();
    }
        break;
    case Pov::West: {
        Real tm = mSky->getTimeMultiplier();
        if (tm > 0) tm = 0;
        else       tm -= 0.1;
        mSky->setTimeMultiplier(tm);
    }
        break;
    case Pov::East: {
        Real tm = mSky->getTimeMultiplier();
        if (tm < 0) tm = 0;
        else       tm += 0.1;
        mSky->setTimeMultiplier(tm);
        /*if (mCurrentStage >= 3) {
            switch (mSkyPreset) {
            case RealisticSky::SKY_DESERT:
                mSkyPreset = RealisticSky::SKY_SUNSET;
                break;
            case RealisticSky::SKY_SUNSET:
                mSkyPreset = RealisticSky::SKY_CLEAR;
                break;
            case RealisticSky::SKY_CLEAR:
                mSkyPreset = RealisticSky::SKY_NIGHT;
                break;
            case RealisticSky::SKY_NIGHT:
                mSkyPreset = RealisticSky::SKY_THUNDER1;
                break;
            case RealisticSky::SKY_THUNDER1:
                mSkyPreset = RealisticSky::SKY_THUNDER2;
                break;
            case RealisticSky::SKY_THUNDER2:
                mSkyPreset = RealisticSky::SKY_DESERT;
                break;
            default:
                break;
            }
            mSky->setPreset(mSkyPreset);*/
    }
        break;
    default:
        break;
    }
}

bool DemoAdvanced::vector3Moved(const Caelum::JoyStickEvent &arg, int index) {
}
