/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "input/psmove/psmovecontroller.h"

using namespace Caelum;

PSMoveController::PSMoveController(OIS::InputManager *creator,
                                   int id,
                                   bool buffered,
                                   PSMoveFactory *local_creator) {
}

PSMoveController::~PSMoveController() {
    this->disconnect();
}





bool PSMoveController::connect() {
    return false;
}

void PSMoveController::disconnect() {
    mCtype = Conn_Unknown;
}

bool PSMoveController::isConnected() {
    return (mCtype != Conn_Unknown);
}
