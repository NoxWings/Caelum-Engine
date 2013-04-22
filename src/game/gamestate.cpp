/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "gamestate.h"

#include "game/gamemanager.h"
#include "game/scenemanager.h"

using namespace Caelum;

GameState::GameState() : mGameMan(0) {
}

GameState::GameState(GameManager *gameman) {
    _setGameManager(gameman);
}

GameState::~GameState() {
}

void GameState::_setGameManager(GameManager *gameman) {
    mGameMan = gameman;
    mSceneManager = mGameMan->getSceneManager();
    mLog = mGameMan->getGameLog();
}
