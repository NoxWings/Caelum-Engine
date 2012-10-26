/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "input/motioncontrollerfactory.h"

using namespace Caelum;

MotionControllerFactory::MotionControllerFactory() {
}

MotionControllerFactory::~MotionControllerFactory() {
    controllers.clear();
}

MotionController*
MotionControllerFactory::createInstance(const std::string& name) {
    // TODO
    return NULL;
}

void
MotionControllerFactory::destroyInstance(MotionController *instance) {
    // TODO
}
