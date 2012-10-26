/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "input/keylistener.h"
#include "input/inputmanager.h"



Caelum::KeyListener::KeyListener() {
}

void Caelum::KeyListener::startListeningKey() {
    InputManager* iMan = InputManager::getSingletonPtr();
    iMan->addKeyListener(this);
}
