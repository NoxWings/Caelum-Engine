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
};

#endif // TESTSTATE_H
