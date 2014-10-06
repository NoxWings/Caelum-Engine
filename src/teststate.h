#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "game/gamestate.h"
#include "render/entity.h"

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

    // JoyStick
    bool buttonPressed(const Caelum::JoyStickEvent &arg, int button);
    bool buttonReleased(const Caelum::JoyStickEvent &arg, int button);
    bool axisMoved(const Caelum::JoyStickEvent &arg, int axis);
    bool sliderMoved(const Caelum::JoyStickEvent &arg, int index);
    bool povMoved(const Caelum::JoyStickEvent &arg, int index);
    bool vector3Moved(const Caelum::JoyStickEvent &arg, int index);

  protected:
    bool mContinue;
    Caelum::GameObject *camobj;
    Caelum::Camera *cam;
    Caelum::Terrain *terrain;

    Caelum::Vector3 mov;
    Caelum::Degree yawRot, pitchRot;
    Caelum::Real axisYaw, axisPitch;
    int movSpeed, rotateSpeed;

    Caelum::Entity *ent;
};

#endif // TESTSTATE_H
