#include "demos/demonetclient.h"

#include "render/rendermanager.h"

#include <net/nethost.h>

using namespace Caelum;

DemoNetClient::DemoNetClient(const Caelum::String &sceneName)
    : Demo(sceneName) {
}

DemoNetClient::~DemoNetClient() {
}

void DemoNetClient::setStage(int n) {
    switch (n) {
    case 0:
        mRenderLayer->setBackgroundColour(Caelum::ColourValue::White);
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

void DemoNetClient::replaceLayout(const String &layout) {
    if (mCurrentLayout.size() > 1) {
        mGuiInstance->unloadLayout(mCurrentLayout);
    }
    mCurrentLayout = layout;
    mGuiInstance->loadLayout(mCurrentLayout, "");
}

void DemoNetClient::setupScene() {
    mClient->connect("localhost", 1234);

    GameObject *obj; Entity *ent; RigidBody *rb;
    obj = mScene->createGameObject("FloorObj");
    ent = mRenderLayer->createEntity("Floor", "ts_dojo.mesh");
    rb  = mPhysicsLayer->createRigidBody("Floor",ent, Vector3::UNIT_SCALE, 0, PhysicsLayer::PHY_SHAPE_TRIMESH);
    obj->attachComponent(ent);
    obj->attachComponent(rb);
    ent->setShadowCast(false);
    obj->pitch(Caelum::Degree(-90));
    obj->setPosition(0,-9,0);
    obj->notifyTransform();

    // Create light
    Light *lt;
    obj = mScene->createGameObject("Light");
    lt = mRenderLayer->createLight("Light", Caelum::Light::LT_POINT);
    obj->attachComponent(lt);
    lt->setDiffuseColour(ColourValue::White);
    lt->setSpecularColour(ColourValue(0.4, 0.4, 0.4));

    // Full stencil shadows
    mRenderLayer->setAmbientLight(Caelum::ColourValue(0.2, 0.2, 0.2));
    mRenderLayer->setShadowTechnique(Caelum::SHADOWTYPE_STENCIL_ADDITIVE);

    mCamObj->setPosition(0,4,15);
}


void DemoNetClient::enter() {
    Demo::enter();
    mCurrentLayout = "";
    mCamMovSpeed = 100;
    mCamJoystickRotateSpeed = 150;
    mCamPitch = mCamYaw = 0;
    mCamAxisPitch = mCamAxisYaw = 0;
    mCamMov = Vector3::ZERO;

    mClient = new NetClient(this);
    setupScene();
    resume();
}

void DemoNetClient::exit() {
    delete mClient;
    Demo::exit();
}

void DemoNetClient::pause() {
    mGuiInstance->pause();
    Demo::pause();
}

void DemoNetClient::resume() {
    Demo::resume();
    mGuiInstance->resume();
    mGuiInstance->setPointerVisible(false);
}

void DemoNetClient::preRender(const Caelum::RenderEvent &evt) {
    Vector3 mov = mCamMov *(mCamMovSpeed * evt.timeSinceLastRender);
    mCamObj->move(mov.x, 0, mov.z, Caelum::GameObject::TS_LOCAL);
    mCamObj->move(0, mov.y, 0, Caelum::GameObject::TS_WORLD);

    mCamYaw = Caelum::Degree(mCamAxisYaw*mCamJoystickRotateSpeed*evt.timeSinceLastRender);
    mCamPitch = Caelum::Degree(mCamAxisPitch*mCamJoystickRotateSpeed*evt.timeSinceLastRender);
    mCamObj->yaw(mCamYaw, Caelum::GameObject::TS_WORLD);
    mCamObj->pitch(mCamPitch, Caelum::GameObject::TS_LOCAL);
    mCamObj->forceUpdate();
}

void DemoNetClient::rendering(const Caelum::RenderEvent &evt) {
    mClient->update();
}

void DemoNetClient::postRender(const Caelum::RenderEvent &evt) {
}

bool DemoNetClient::mouseMoved(const MouseEvent &evt) {
    return true;
}

bool DemoNetClient::mousePressed(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoNetClient::mouseReleased(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoNetClient::keyPressed(const KeyEvent &evt) {
    return true;
}

bool DemoNetClient::keyReleased(const KeyEvent &evt) {
    if (evt.key == Caelum::KC_ESCAPE) {
        mContinue = false;
    }
    return true;
}

bool DemoNetClient::buttonPressed(const Caelum::JoyStickEvent &arg, int button) {
    if (button == 4) { // L1 previous
        previousStage();
    } else if (button == 5) { // R1 next
        nextStage();
    } else if (button == 1) {
        mPhysicsLayer->setDebugMode(!mPhysicsLayer->getDebugMode());
    }
}

bool DemoNetClient::buttonReleased(const Caelum::JoyStickEvent &arg, int button) {
}

bool DemoNetClient::axisMoved(const Caelum::JoyStickEvent &arg, int axis) {
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
    }
    return true;
}

bool DemoNetClient::sliderMoved(const Caelum::JoyStickEvent &arg, int index) {
}

bool DemoNetClient::povMoved(const Caelum::JoyStickEvent &arg, int index) {
}

bool DemoNetClient::vector3Moved(const Caelum::JoyStickEvent &arg, int index) {
}
