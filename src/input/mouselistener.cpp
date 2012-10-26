/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "input/mouselistener.h"
#include "input/inputmanager.h"


Caelum::MouseListener::MouseListener() {
}

void Caelum::MouseListener::startListeningMouse() {
    InputManager* iMan = InputManager::getSingletonPtr();
    iMan->addMouseListener(this);
}

