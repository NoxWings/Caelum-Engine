#ifndef DEMOADVANCED_H
#define DEMOADVANCED_H

#include "demos/demo.h"

class DemoAdvanced : public Demo {
  public:
    DemoAdvanced(const Caelum::String& sceneName);
    ~DemoAdvanced();

    // ***************************
    // DemoState virtual functions
    // ***************************
    void setStage(int n);
    void replaceLayout(const Caelum::String &layout);
    void setCollisionType(Caelum::PhysicsLayer::PHY_SHAPE_TYPE type);
    void clearObjects();
    void launchObject(Caelum::GameObject *ref, Caelum::Real force);
    void activateMusic();

    void setupActualScene();
    void createTerrain();

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
    // physics
    Caelum::String mCurrentLayout;
    Caelum::GameObject *mPlatformObj;
    Caelum::Entity *mPlatform;
    Caelum::RigidBody *mPlatformPhy;
    Caelum::Real mLaunchPitch, mLaunchRoll;
    Caelum::PhysicsLayer::PHY_SHAPE_TYPE mCollisionType;
    Caelum::GameObject *mFountain1,*mFountain2;
    bool mOpenFountains;
    Caelum::Real mOpenFountainsTime;

    Caelum::RealisticSky::SKY_PRESET_ENUM mSkyPreset;
    Caelum::AudioSource *mMusicSource;
    Caelum::RealisticSky *mSky;
    Caelum::OceanSimulator *mOcean;
    Caelum::Terrain *mTerrain;
};

static int barrelAdvancedCount = 0;

#endif // DEMOADVANCED_H
