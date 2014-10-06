#ifndef DEMONETCLIENT_H
#define DEMONETCLIENT_H


#include "demos/demo.h"

namespace Caelum {
class NetClient;
}

class DemoNetClient : public Demo {
  public:
    DemoNetClient(const Caelum::String& sceneName);
    virtual ~DemoNetClient();

    // ***************************
    // DemoState virtual functions
    // ***************************
    void setStage(int n);
    void replaceLayout(const Caelum::String &layout);

    void setupScene();

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
    // Window Listener Interface
    // ****************************
    void windowResized(Caelum::RenderWindow* rw) { }

    // ****************************
    // Mouse Listener
    // ****************************
    bool mouseMoved    (const Caelum::MouseEvent& evt);
    bool mousePressed  (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);
    bool mouseReleased (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id);
    bool mouseClicked  (const Caelum::MouseEvent& evt, Caelum::MouseButtonID id) { return true;}

    // ****************************
    // Keyboard
    // ****************************
    bool keyPressed  (const Caelum::KeyEvent &evt);
    bool keyReleased (const Caelum::KeyEvent &evt);
    bool keyTap      (const Caelum::KeyEvent &evt) { return true;}

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
    Caelum::String mCurrentLayout;
    Caelum::NetClient *mClient;

    Caelum::GameObject *mObj;
    Caelum::Entity *mEnt;
    Caelum::RigidBody *mRb;
};
#endif // DEMONETCLIENT_H
