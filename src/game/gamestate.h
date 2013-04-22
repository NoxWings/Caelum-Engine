/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "EnginePrerequisites.h"

#include "game/scene.h"
#include "core/log.h"
#include "game/eventlistener.h"

namespace Caelum {

/** Fordward declaration **/
class GameManager;
class SceneManager;

class GameState : public EventListener {
  public:
    GameState();
    GameState(GameManager *gameman);
    virtual ~GameState();

    /**
     * @brief setGameManager is a function called everytime u register a gamestate into
     * a the gamemanager so the gamestate can iteract with the game engine
     * @param gameman
     *        The gamemanager to iteract with the engine
     */
    void _setGameManager(GameManager* gameman);

    // ***************************
    // GameState virtual functions
    // ***************************
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    // *************************
    // Window Listener Interface
    // *************************
    //virtual void windowMoved(RenderWindow* rw);
    //virtual void windowResized(RenderWindow* rw);
    //virtual bool windowClosing(RenderWindow* rw);
    //virtual void windowClosed(RenderWindow* rw);
    //virtual void windowFocusChange(RenderWindow* rw);

    // ****************************
    // Rendering Listener Interface
    // ****************************
    //virtual bool preRenderUpdate(const RenderEvent& evt);
    //virtual bool renderingUpdate(const RenderEvent& evt);
    //virtual bool postRenderUpdate(const RenderEvent& evt);

  protected:
    GameManager *mGameMan;
    SceneManager *mSceneManager;
    Scene *mScene;
    Log *mLog;
};

}
#endif // GAMESTATE_H
