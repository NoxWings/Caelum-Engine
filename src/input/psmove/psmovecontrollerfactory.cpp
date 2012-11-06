/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "input/psmovecontrollerfactory.h"

using namespace Caelum;

PSMoveControllerFactory::PSMoveControllerFactory() {
}

PSMoveControllerFactory::~PSMoveControllerFactory() {
    controllers.clear();
}

PSMoveController*
PSMoveControllerFactory::createInstance(const std::string& name) {
    // TODO
    return NULL;
}

void
PSMoveControllerFactory::destroyInstance(PSMoveController *instance) {
    // TODO
}
