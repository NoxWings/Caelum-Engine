#include "demodesign.h"

#include "render/rendermanager.h"

using namespace Caelum;

DemoDesign::DemoDesign(const Caelum::String &sceneName)
    : Demo(sceneName) {
}

DemoDesign::~DemoDesign() {
}

void DemoDesign::setStage(int n) {
    switch (n) {
    case 0:
        mRenderLayer->setBackgroundColour(Caelum::ColourValue::White);
        replaceLayout("Design_intro");
        break;
    case 1:
        replaceLayout("Design_nucleo");
        break;
    case 2:
        replaceLayout("Design_entrada");
        break;
    case 3:
        replaceLayout("Design_estados");
        break;
    case 4:
        replaceLayout("Design_escenas");
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

void DemoDesign::replaceLayout(const String &layout) {
    if (mCurrentLayout.size() > 1) {
        mGuiInstance->unloadLayout(mCurrentLayout);
    }
    mCurrentLayout = layout;
    mGuiInstance->loadLayout(mCurrentLayout, "");
}

void DemoDesign::enter() {
    Demo::enter();
    mCurrentLayout = "";
    resume();
}

void DemoDesign::exit() {
    Demo::exit();
}

void DemoDesign::pause() {
    mGuiInstance->pause();
    Demo::pause();
}

void DemoDesign::resume() {
    Demo::resume();
    mGuiInstance->resume();
    mGuiInstance->setPointerVisible(false);
}

void DemoDesign::preRender(const Caelum::RenderEvent &evt) {
}

void DemoDesign::rendering(const Caelum::RenderEvent &evt) {
}

void DemoDesign::postRender(const Caelum::RenderEvent &evt) {
}

bool DemoDesign::mouseMoved(const MouseEvent &evt) {
    return true;
}

bool DemoDesign::mousePressed(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoDesign::mouseReleased(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoDesign::keyPressed(const KeyEvent &evt) {
    return true;
}

bool DemoDesign::keyReleased(const KeyEvent &evt) {
    if (evt.key == Caelum::KC_ESCAPE) {
        mContinue = false;
    }
    return true;
}

bool DemoDesign::buttonPressed(const Caelum::JoyStickEvent &arg, int button) {
    if (button == 4) { // L1 previous
        previousStage();
    } else if (button == 5) { // R1 next
        nextStage();
    }
}

bool DemoDesign::buttonReleased(const Caelum::JoyStickEvent &arg, int button) {
}

bool DemoDesign::axisMoved(const Caelum::JoyStickEvent &arg, int axis) {
}

bool DemoDesign::sliderMoved(const Caelum::JoyStickEvent &arg, int index) {
}

bool DemoDesign::povMoved(const Caelum::JoyStickEvent &arg, int index) {
}

bool DemoDesign::vector3Moved(const Caelum::JoyStickEvent &arg, int index) {
}
