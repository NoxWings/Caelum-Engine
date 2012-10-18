/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of NAGE.
 */

#include "input/keylistener.h"
#include "input/inputmanager.h"



NAGE::KeyListener::KeyListener() {
}

void NAGE::KeyListener::startListeningKey() {
    InputManager* iMan = InputManager::getSingletonPtr();
    iMan->addKeyListener(this);
}
