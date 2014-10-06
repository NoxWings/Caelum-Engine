#include "demos/demointro.h"

#include "render/rendermanager.h"

using namespace Caelum;

DemoIntro::DemoIntro(const Caelum::String &sceneName)
    : Demo(sceneName) {
}

DemoIntro::~DemoIntro() {
}

void DemoIntro::setStage(int n) {
    switch (n) {
    case 0:
        mRenderLayer->setBackgroundColour(Caelum::ColourValue::White);
        replaceLayout("Intro_problema");
        break;
    case 1:
        replaceLayout("Intro_caelum");
        break;
    case 2:
        replaceLayout("Intro_modelo");
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

void DemoIntro::replaceLayout(const String &layout) {
    if (mCurrentLayout.size() > 1) {
        mGuiInstance->unloadLayout(mCurrentLayout);
    }
    mCurrentLayout = layout;
    mGuiInstance->loadLayout(mCurrentLayout, "");
}

void DemoIntro::enter() {
    Demo::enter();
    mCurrentLayout = "";
    resume();
}

void DemoIntro::exit() {
    Demo::exit();
}

void DemoIntro::pause() {
    mGuiInstance->pause();
    Demo::pause();
}

void DemoIntro::resume() {
    Demo::resume();
    mGuiInstance->resume();
    mGuiInstance->setPointerVisible(false);
}

void DemoIntro::preRender(const Caelum::RenderEvent &evt) {
}

void DemoIntro::rendering(const Caelum::RenderEvent &evt) {
}

void DemoIntro::postRender(const Caelum::RenderEvent &evt) {
}

bool DemoIntro::mouseMoved(const MouseEvent &evt) {
    return true;
}

bool DemoIntro::mousePressed(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoIntro::mouseReleased(const MouseEvent &evt, MouseButtonID id) {
    return true;
}

bool DemoIntro::keyPressed(const KeyEvent &evt) {
    return true;
}

bool DemoIntro::keyReleased(const KeyEvent &evt) {
    if (evt.key == Caelum::KC_ESCAPE) {
        mContinue = false;
    }
    return true;
}

bool DemoIntro::buttonPressed(const Caelum::JoyStickEvent &arg, int button) {
    if (button == 4) { // L1 previous
        previousStage();
    } else if (button == 5) { // R1 next
        nextStage();
    }
}

bool DemoIntro::buttonReleased(const Caelum::JoyStickEvent &arg, int button) {
}

bool DemoIntro::axisMoved(const Caelum::JoyStickEvent &arg, int axis) {
}

bool DemoIntro::sliderMoved(const Caelum::JoyStickEvent &arg, int index) {
}

bool DemoIntro::povMoved(const Caelum::JoyStickEvent &arg, int index) {
}

bool DemoIntro::vector3Moved(const Caelum::JoyStickEvent &arg, int index) {
}
