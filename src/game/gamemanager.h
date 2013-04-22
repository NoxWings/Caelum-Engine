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
    bool preRenderUpdate(const RenderEvent& evt)  { return mCurrentState->preRenderUpdate(evt);}
    bool renderingUpdate(const RenderEvent& evt)  { return mCurrentState->renderingUpdate(evt);}
    bool postRenderUpdate(const RenderEvent& evt) { return mCurrentState->postRenderUpdate(evt);}

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