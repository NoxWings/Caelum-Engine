#include "demophysics.h"

#include "render/rendermanager.h"

#include <OGRE/OgreLight.h>

using namespace Caelum;

DemoPhysics::DemoPhysics(const Caelum::String &sceneName)
    : Demo(sceneName) {
}

DemoPhysics::~DemoPhysics() {
}

void DemoPhysics::setStage(int n) {
    switch (n) {
    case 0:
        mRenderLayer->setBackgroundColour(Caelum::ColourValue::White);
        replaceLayout("Physics_brief");
        break;
    case 1:
        replaceLayout("Physics_mallas");
        break;
    case 2:
        replaceLayout("Physics_contador");
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

void DemoPhysics::replaceLayout(const String &layout) {
    if (mCurrentLayout.size() > 1) {
        mGuiInstance->unloadLayout(mCurrentLayout);
    }
    mCurrentLayout = layout;
    mGuiInstance->loadLayout(mCurrentLayout, "");
}

void DemoPhysics::setCollisionType(PhysicsLayer::PHY_SHAPE_TYPE type) {
    if (type != mCollisionType) {
        //clearObjects();
        mCollisionType = type;
    }
    mLog->logMessage("Collision type completely set!");
}

void DemoPhysics::clearObjects(){
    if (mCurrentStage < 2) return;
    while (barrelcount > 0) {
        static String counter;          // string which will contain the result
        std::ostringstream convert;   // stream used for the conversion
        convert << barrelcount-1;      // insert the textual representation of 'Number' in the characters in the stream
        counter = convert.str(); // set 'Result' to the contents of the stream

        // Object creation
        mPhysicsLayer->destroyRigidBody(String("barrelphy")+counter);
        mScene->getRootObject()->destroyChildGameObject(String("barrelobj")+counter);
        mRenderLayer->destroyComponent(String("barrel")+counter);
        barrelcount--;
    }
    barrelcount = 0;
    MyGUI::TextBox* tb = mGuiInstance->getGUI()->findWidget<MyGUI::TextBox>("ObjCounter");
    tb->setCaption("#0000ffObjetos:  #ffffff0");
}

void DemoPhysics::launchObject(Caelum::GameObject *ref, Real force) {
    if (mCurrentStage < 2) return;
    static String count;          // string which will contain the result
    std::ostringstream convert;   // stream used for the conversion
    convert << barrelcount++;      // insert the textual representation of 'Number' in the characters in the stream
    count = convert.str(); // set 'Result' to the contents of the stream

    MyGUI::TextBox* tb = mGuiInstance->getGUI()->findWidget<MyGUI::TextBox>("ObjCounter");
    tb->setCaption("#0000ffObjetos:  #ffffff"+StringUtils::to_string(barrelcount));

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


void DemoPhysics::enter() {
    Demo::enter();
    mCurrentLayout = "";
    mCamMovSpeed = 100;
    mCamJoystickRotateSpeed = 150;
    mCamPitch = mCamYaw = 0;
    mCamAxisPitch = mCamAxisYaw = 0;
    mLaunchPitch = mLaunchRoll = 0;
    mCamMov = Vector3::ZERO;
    mCollisionType = Caelum::PhysicsLayer::PHY_SHAPE_CONVEX;
    barrelcount = 0;

    mScene->getRenderLayer()->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
    mScene->getRenderLayer()->setShadowTechnique(Caelum::SHADOWTYPE_STENCIL_MODULATIVE);
//    mScene->getRenderLayer()->setShadowProjectionType(Caelum::SHADOW_PROJECTION_LISPSM);
//    mScene->getRenderLayer()->setShadowFarDistance(400);
//    mScene->getRenderLayer()->setShadowTextureSettings(1024, 1);
    GameObject *sunObj = mScene->createGameObject("SunObj");
    Light* sunLight = mScene->getRenderLayer()->createLight("SunLight", Light::LT_DIRECTIONAL);
    sunLight->_getActualLight()->setDirection(0.55, -1.5, -0.75);
    sunObj->attachComponent(sunLight);
    sunLight->setDiffuseColour(ColourValue::White);
    sunLight->setSpecularColour(ColourValue(0.4, 0.4, 0.4));

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

    mPlatformObj = mScene->createGameObject("Platform");
    mPlatform = mRenderLayer->createEntity("PlatformLevel", "TestLevel_b0.mesh");
    mPlatform->setShadowCast(false);
    mPlatformPhy = mPhysicsLayer->createRigidBody("PlatformPhy", mPlatform, Vector3::UNIT_SCALE, 0, PhysicsLayer::PHY_SHAPE_TRIMESH);
    mPlatformObj->attachComponent(mPlatform);
    mPlatformObj->attachComponent(mPlatformPhy);
    mPlatformObj->move(Vector3(0, -100, 0));
    mPlatformObj->setScale(Vector3::UNIT_SCALE*10);
    mPlatformObj->notifyTransform();

    mCamObj->setPosition(100,0,100);
    mCamObj->lookAt(mPlatformObj);

    mFountain1 = mScene->createGameObject("fountain1");
    mFountain2 = mScene->createGameObject("fountain2");
    mFountain1->setPosition(100, 0, 100);
    mFountain2->setPosition(-100,0,-100);
    mFountain1->lookAt(mPlatformObj);
    mFountain2->lookAt(mPlatformObj);
    mFountain1->pitch(Degree(30));
    mFountain2->pitch(Degree(30));
    mOpenFountains = false;
    mOpenFountainsTime = 0;
    resume();
}

void DemoPhysics::exit() {
    Demo::exit();
}

void DemoPhysics::pause() {
    mGuiInstance->pause();
    Demo::pause();
}

void DemoPhysics::resume() {
    Demo::resume();
    mGuiInstance->resume();
    mGuiInstance->setPointerVisible(false);
}

void DemoPhysics::preRender(const Caelum::RenderEvent &evt) {
    //mCamMov.normalise();
    Vector3 mov = mCamMov *(mCamMovSpeed * evt.timeSinceLastRender);
    mCamObj->move(mov.x, 0, mov.z, Caelum::GameObject::TS_LOCAL);
    mCamObj->move(0, mov.y, 0, Caelum::GameObject::TS_WORLD);

    mCamYaw = Caelum::Degree(mCamAxisYaw*mCamJoystickRotateSpeed*evt.timeSinceLastRender);
    mCamPitch = Caelum::Degree(mCamAxisPitch*mCamJoystickRotateSpeed*evt.timeSinceLastRender);
    mCamObj->yaw(mCamYaw, Caelum::GameObject::TS_WORLD);
    mCamObj->pitch(mCamPitch, Caelum::GameObject::TS_LOCAL);
    mCamObj->forceUpdate();

    if (mOpenFountains) {
        launchObject(mFountain1, 30);
        launchObject(mFountain2, 30);
    }
}

void DemoPhysics::rendering(const Caelum::RenderEvent &evt) {
    //mPhysicsLayer->update(evt.timeSinceLastRender*2);
}

void DemoPhysics::postRender(const Caelum::RenderEvent &evt) {
}

bool DemoPhysics::mouseMoved(const MouseEvent &evt) {
    return true;
}

bool DemoPhysics::mousePressed(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoPhysics::mouseReleased(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoPhysics::keyPressed(const KeyEvent &evt) {
    return true;
}

bool DemoPhysics::keyReleased(const KeyEvent &evt) {
    if (evt.key == Caelum::KC_ESCAPE) {
        mContinue = false;
    }
    return true;
}

bool DemoPhysics::buttonPressed(const Caelum::JoyStickEvent &arg, int button) {
    if (button == 4) { // L1 previous
        previousStage();
    } else if (button == 5) { // R1 next
        nextStage();
    } else if (button == 7) { // R2 debug physics
        bool debug = mPhysicsLayer->getDebugMode();
        mPhysicsLayer->setDebugMode(!debug);
    } else if (button == 6) {
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
    } else if ((button == 1 || button == 3) && mCurrentStage >= 2) {
        mOpenFountains = true;
    }
}

bool DemoPhysics::buttonReleased(const Caelum::JoyStickEvent &arg, int button) {
    if (button == 1 || button == 3) {
        mOpenFountains = false;
    }
}

bool DemoPhysics::axisMoved(const Caelum::JoyStickEvent &arg, int axis) {
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

bool DemoPhysics::sliderMoved(const Caelum::JoyStickEvent &arg, int index) {

}

bool DemoPhysics::povMoved(const Caelum::JoyStickEvent &arg, int index) {
    switch (arg.mState.mPOV[index].direction) {
    case Pov::North:
        setCollisionType(PhysicsLayer::PHY_SHAPE_CONVEX);
        break;
    case Pov::South:
        //setCollisionType(PhysicsLayer::PHY_SHAPE_CYLINDER);
        clearObjects();
        break;
    case Pov::West:
        setCollisionType(PhysicsLayer::PHY_SHAPE_SPHERE);
        break;
    case Pov::East:
        setCollisionType(PhysicsLayer::PHY_SHAPE_BOX);
        break;
    default:
        break;
    }
}

bool DemoPhysics::vector3Moved(const Caelum::JoyStickEvent &arg, int index) {
}
