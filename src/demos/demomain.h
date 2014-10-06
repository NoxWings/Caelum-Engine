#ifndef DEMOS_DEMOMAIN_H
#define DEMOS_DEMOMAIN_H

#include "demos/demo.h"

#include <MyGUI/MyGUI_Button.h>

class DemoMain : public Demo {
  public:
    DemoMain(const Caelum::String& sceneName);
    ~DemoMain();

    // ***************************
    // DemoState virtual functions
    // ***************************
    void setStage(int n);
    void stage0();
    void selectButton(int n);

    void createHydrax();
    void destroyHydrax();

    // ***************************
    // GUI callback functions
    // ***************************
    void buttonExit(MyGUI::WidgetPtr _sender);

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
    // Demo main
    Caelum::GameObject *mSunObj;
    Caelum::Light *mSun;
    Caelum::RealisticSky *mSky;
    Caelum::AudioSource *mMusicSource;
    MyGUI::Button *mButton;
    int mIndex;
    Caelum::OceanSimulator *mOcean;
    Caelum::Terrain *mTerrain;
};


#endif // DEMOS_DEMOMAIN_H
