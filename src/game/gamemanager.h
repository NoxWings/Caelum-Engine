/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_GAME_GAMEMANAGER_H
#define SRC_GAME_GAMEMANAGER_H

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "game/eventlistener.h"

#include "core/log.h"
#include "game/scenemanager.h"
#include "game/gamestate.h"

#include "patterns/AssociativeCollection.h"
#include <vector>

namespace Caelum {

/** Fordward declarations **/
class GameEngine;

class GameManager : public Singleton<GameManager>, public EventListener {
  public:
    GameManager();
    GameManager(GameEngine* engine);
    ~GameManager();

    void addState(const String& name, GameState * state, bool initialState = false);

    void start();
    void startServerLoop();
    void transitionTo(const String& stateName);
    void transitionBack();

    Log* getGameLog() {return mLog;}
    SceneManager*  getSceneManager()  { return mSceneMan;}


    // *****************************************************************
    //                 Event Listener Interface
    // *****************************************************************

    // Window Listener Interface
    void windowMoved(RenderWindow* rw)   { mCurrentState->windowMoved(rw);}
    void windowResized(RenderWindow* rw) { mCurrentState->windowResized(rw);}
    bool windowClosing(RenderWindow* rw) { return mCurrentState->windowClosing(rw);}
    void windowClosed(RenderWindow* rw)  { mCurrentState->windowClosed(rw);}
    void windowFocusChange(RenderWindow* rw) { mCurrentState->windowFocusChange(rw);}

    // Rendering Listener Interface
    bool preRenderUpdate(const RenderEvent& evt);
    bool renderingUpdate(const RenderEvent& evt)  { return mCurrentState->renderingUpdate(evt);}
    bool postRenderUpdate(const RenderEvent& evt) { return mCurrentState->postRenderUpdate(evt);}

    // Mouse Listener
    bool mouseMoved    (const MouseEvent& evt) { if (mCurrentState) mCurrentState->mouseMoved(evt); return true;}
    bool mousePressed  (const MouseEvent& evt, MouseButtonID id) { if (mCurrentState) mCurrentState->mousePressed(evt, id); return true;}
    bool mouseReleased (const MouseEvent& evt, MouseButtonID id) { if (mCurrentState) mCurrentState->mouseReleased(evt, id); return true;}
    bool mouseClicked  (const MouseEvent& evt, MouseButtonID id) { if (mCurrentState) mCurrentState->mouseClicked(evt, id); return true;}

    // Keyboard
    bool keyPressed  (const KeyEvent &evt) { if (mCurrentState) mCurrentState->keyPressed(evt); return true;}
    bool keyReleased (const KeyEvent &evt) { if (mCurrentState) mCurrentState->keyReleased(evt); return true;}
    bool keyTap      (const KeyEvent &evt) { if (mCurrentState) mCurrentState->keyTap(evt); return true;}

    // JoyStick
    bool buttonPressed(const JoyStickEvent &arg, int button)  { if (mCurrentState) mCurrentState->buttonPressed(arg, button); return true;}
    bool buttonReleased(const JoyStickEvent &arg, int button) { if (mCurrentState) mCurrentState->buttonReleased(arg, button); return true;}
    bool axisMoved(const JoyStickEvent &arg, int axis)     { if (mCurrentState) mCurrentState->axisMoved(arg, axis); return true;}
    bool sliderMoved(const JoyStickEvent &arg, int index)  { if (mCurrentState) mCurrentState->sliderMoved(arg, index); return true;}
    bool povMoved(const JoyStickEvent &arg, int index)     { if (mCurrentState) mCurrentState->povMoved(arg, index); return true;}
    bool vector3Moved(const JoyStickEvent &arg, int index) { if (mCurrentState) mCurrentState->vector3Moved(arg, index); return true;}

  private:
    void _init(GameEngine *engine);

    GameEngine *mEngine;
    Log *mLog;
    SceneManager *mSceneMan;

    GameState *mCurrentState;
    AssociativeCollection<String, GameState*> mGameStates;
    std::vector<GameState*> mGameStateQueue;
};

}

#endif // GAMEMANAGER_H
