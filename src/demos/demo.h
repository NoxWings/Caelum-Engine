#ifndef DEMOS_DEMO_H
#define DEMOS_DEMO_H

#include "game/gamestate.h"
#include "game/gamemanager.h"
#include "game/scenemanager.h"
#include "render/rendermanager.h"
#include "render/guimanager.h"

#include <MyGUI/MyGUI.h>

namespace Caelum {
class NetServer;
class NetClient;
}

class Demo : public Caelum::GameState {
  public:
    Demo(const Caelum::String& sceneName);
    ~Demo();

    // ***************************
    // DemoState virtual functions
    // ***************************
    void nextStage() { setStage(++mCurrentStage);}
    void previousStage() { setStage(--mCurrentStage);}
    virtual void setStage(int n) = 0;

    // ***************************
    // GameState virtual functions
    // ***************************
    virtual void enter();
    virtual void exit();

    virtual void pause();
    virtual void resume();

    // ****************************
    // Rendering Listener Interface
    // ****************************
    virtual void preRender(const Caelum::RenderEvent& evt) = 0;
    virtual void rendering(const Caelum::RenderEvent& evt) = 0;
    virtual void postRender(const Caelum::RenderEvent& evt) = 0;

    bool preRenderUpdate(const Caelum::RenderEvent& evt) { preRender(evt); return mContinue;}
    bool renderingUpdate(const Caelum::RenderEvent& evt) {
        rendering(evt);
        mRenderLayer->update(evt.timeSinceLastRender);
        mPhysicsLayer->update(evt.timeSinceLastRender*3);
        return mContinue;
    }
    bool postRenderUpdate(const Caelum::RenderEvent& evt) { postRender(evt); return mContinue;}

  private:
    Caelum::String mSceneName;
  protected:
    friend class Caelum::NetServer;
    friend class Caelum::NetClient;

    int mCurrentStage;
    bool mSceneOwner;
    // loop continue
    bool mContinue;
    // Scene management
    Caelum::RenderLayer  *mRenderLayer;
    Caelum::GuiInstance  *mGuiInstance;
    Caelum::PhysicsLayer *mPhysicsLayer;
    Caelum::AudioLayer   *mAudioLayer;
    // Camera
    Caelum::GameObject *mCamObj, *mCamTarget;
    Caelum::Camera *mCam;
    Caelum::Vector3 mCamMov;
    Caelum::Degree mCamYaw, mCamPitch;
    Caelum::Real mCamAxisYaw, mCamAxisPitch;
    int mCamMovSpeed, mCamMouseRotateSpeed, mCamJoystickRotateSpeed;
};

#endif // DEMO_H
