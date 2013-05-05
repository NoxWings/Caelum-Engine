/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "gamemanager.h"

#include "core/gameengine.h"

using namespace Caelum;

GameManager::GameManager() {
    this->_init(GameEngine::getSingletonPtr());
}

GameManager::GameManager(GameEngine* engine) : mCurrentState(0) {
    this->_init(engine);
}

void GameManager::_init(GameEngine *engine) {
    // save the game engine
    mEngine = engine;
    // create the log
    mLog = mEngine->getLogManager()->createLog("GameManager.log");
    mLog->logMessage("GAMEMANAGER: Initialising game manager");
    // create the scene manager
    mSceneMan = new SceneManager();

    // Register listeners
    //   * Render events
    RenderManager* renderMan = mEngine->getRenderManager();
    renderMan->addWindowEventListener(this);
    renderMan->addRenderListener(this);
    //   * Input events
    InputManager* inputMan = mEngine->getInputManager();
    inputMan->addMouseListener(this);
    inputMan->addKeyListener(this);
}

GameManager::~GameManager() {
    mLog->logMessage("GAMEMANAGER: Shutting down game manager");

    // Unregister listeners
    InputManager* inputMan = mEngine->getInputManager();
    inputMan->removeMouseListener(this);
    inputMan->removeKeyListener(this);

    RenderManager* renderMan = mEngine->getRenderManager();
    renderMan->removeRenderListener(this);
    renderMan->removeWindowEventListener(this);

    // destroy de scene manager
    delete mSceneMan;

    mLog->logMessage("GAMEMANAGER: Shut down complete!");
    // destroy log
    GameEngine::getSingletonPtr()->getLogManager()->destroyLog(mLog);
    mLog = 0;

    // dereference the engine
    mEngine = 0;
}

void GameManager::addState(const String &name, GameState *state, bool initialState) {
    // set the gamemanager into the state so it can work
    state->_setGameManager(this);

    // Add state to the state map
    mGameStates.addItem(name, state);

    // Set the initial currentState
    if (initialState) {
        mCurrentState = state;
        mLog->logMessage("GAMEMANAGER: initial state successfuly added");
    } else {
        mLog->logMessage("GAMEMANAGER: new state successfully added");
    }
}

void GameManager::start() {
    if (mCurrentState) {
        // Setup the current state
        mCurrentState->enter();
        mEngine->getRenderManager()->startRendering();
    } else {
        mLog->logMessage("GAMEMANAGER: couldn't' start the main loop because there is no initial state!");
    }
}

void GameManager::startServerLoop() {
    // TODO make a custom loop not centered in rendering callbacks
}

void GameManager::transitionTo(const String &stateName) {
    GameState *newState = mGameStates.findItem(stateName);

    if (newState) {
        mCurrentState->pause(); // pause the old state
        mGameStateQueue.push_back(mCurrentState); // push old state to the queue

        // Init the new state
        mCurrentState = newState;
        mCurrentState->enter();
    } else {
        mLog->logMessage("GAMEMANAGER: couldn't' found \""+stateName+"\" to make a transition.");
    }
}

void GameManager::transitionBack() {
    if (!mGameStateQueue.empty()) {
        mCurrentState->exit();
        // retrive and resume the previous state
        mCurrentState = mGameStateQueue.back();
        mGameStateQueue.pop_back();
        mCurrentState->resume();
    } else {
        // STOP LOOPING
        mLog->logMessage("WARNING: Trying to make a backtracking transition but the queue is empty!!");
    }
}

bool GameManager::preRenderUpdate(const RenderEvent &evt) {
    mEngine->getInputManager()->update();
    return mCurrentState->preRenderUpdate(evt);
}

