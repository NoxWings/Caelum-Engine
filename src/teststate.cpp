#include "teststate.h"

#include "game/scenemanager.h"

using namespace Caelum;

void TestState::enter() {
    mLog->logMessage("GAMEMANAGER: Creating Test State.");
    mScene = mSceneManager->createScene("TestScene","EXTERIOR_CLOSE");
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
    return false;
}

bool TestState::renderingUpdate(const Caelum::RenderEvent &evt) {
    return false;
}

bool TestState::postRenderUpdate(const Caelum::RenderEvent &evt) {
    return false;
}
