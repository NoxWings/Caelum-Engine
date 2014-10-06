#ifndef DEMOS_DEMOMESH_H
#define DEMOS_DEMOMESH_H

#include "demos/demo.h"

class DemoMesh : public Demo {
  public:
    DemoMesh(const Caelum::String& sceneName);
    ~DemoMesh();

    // ***************************
    // DemoState virtual functions
    // ***************************
    void setStage(int n);
    void stage0();
    void stencilShadows();
    void textureShadows();
    void changePlayer();
    void replaceLayout(const Caelum::String &layout);

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
    void preRender(const Caelum::RenderEvent& evt);
    void rendering(const Caelum::RenderEvent& evt);
    void postRender(const Caelum::RenderEvent& evt);

    // ****************************
    // Mouse Listener
    // ****************************
    bool mouseMoved    (const Caelum::MouseEvent& evt);
    bool mousePressed  (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);
    bool mouseReleased (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);
    bool mouseClicked  (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);

    // ****************************
    // Keyboard
    // ****************************
    bool keyPressed  (const Caelum::KeyEvent &evt);
    bool keyReleased (const Caelum::KeyEvent &evt);
    bool keyTap      (const Caelum::KeyEvent &evt);

    // ****************************
    // JoyStick
    // ****************************
    bool buttonPressed(const Caelum::JoyStickEvent &arg, int button);
    bool buttonReleased(const Caelum::JoyStickEvent &arg, int button);
    bool axisMoved(const Caelum::JoyStickEvent &arg, int axis);
    bool sliderMoved(const Caelum::JoyStickEvent &arg, int index);
    bool povMoved(const Caelum::JoyStickEvent &arg, int index);
    bool vector3Moved(const Caelum::JoyStickEvent &arg, int index);

  protected:
    // Demo mesh
    Caelum::GameObject *mEntObj, *mFloorObj, *mLightObj, *mLightTarget;
    Caelum::Entity *mEnt, *mEnt2, *mFloor;
    Caelum::Light *mLight;
    Caelum::Real mBlendFactor;

    Caelum::String mCurrentLayout;

    float mCamRotation, mLightRotation;
    int mLightRotationSpeed;
};

#endif // DEMOS_DEMOMESH_H
