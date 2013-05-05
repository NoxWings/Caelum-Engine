#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "game/gamestate.h"

class TestState : public Caelum::GameState {
  public:

    // ***************************
    // GameState virtual functions
    // ***************************
    void enter();
    void exit();

    void pause();
    void resume();

    // ****************************
    // Rendering Listener Interface
    // ****************************
    bool preRenderUpdate(const Caelum::RenderEvent& evt);
    bool renderingUpdate(const Caelum::RenderEvent& evt);
    bool postRenderUpdate(const Caelum::RenderEvent& evt);

    // Mouse Listener
    bool mouseMoved    (const Caelum::MouseEvent& evt);
    bool mousePressed  (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);
    bool mouseReleased (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);
    bool mouseClicked  (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);

    // Keyboard
    bool keyPressed  (const Caelum::KeyEvent &evt);
    bool keyReleased (const Caelum::KeyEvent &evt);
    bool keyTap      (const Caelum::KeyEvent &evt);

  protected:
    bool mContinue;
    Caelum::GameObject *camobj;
    Caelum::CameraComponent *cam;

    Caelum::Vector3 mov;
    Caelum::Degree yawRot, pitchRot;
    int movSpeed, rotateSpeed;
};

#endif // TESTSTATE_H
