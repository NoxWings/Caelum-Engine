/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of NAGE.
 */

#include "input/mouselistener.h"
#include "input/inputmanager.h"


NAGE::MouseListener::MouseListener() {
}

void NAGE::MouseListener::startListeningMouse() {
    InputManager* iMan = InputManager::getSingletonPtr();
    iMan->addMouseListener(this);
}

