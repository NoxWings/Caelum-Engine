/*
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  This file is part of NAGE.
 */

#include "input/mouselistener.hpp"
#include "input/inputmanager.hpp"

using namespace NAGE;

MouseListener::MouseListener() {
}

void MouseListener::startListeningMouse() {
    InputManager* iMan = InputManager::getSingletonPtr();
    iMan->addMouseListener(this);
}

