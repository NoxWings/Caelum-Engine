/*
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  This file is part of NAGE.
 */

#include "input/keylistener.hpp"
#include "input/inputmanager.hpp"

using namespace NAGE;

KeyListener::KeyListener() {
}

void KeyListener::startListeningKey() {
    InputManager* iMan = InputManager::getSingletonPtr();
    iMan->addKeyListener(this);
}
