/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum. */


#include "core/gameengine.h"
#include "core/log.h"

int main() {
    Caelum::GameEngine* ce = new Caelum::GameEngine();

    // You can set manual resources here through resourcemanager

    // initialise engine
    ce->setup();

    // create manual window if it is not selected to be created automatically
    // initialize resources manually


    // shutdown engine
    ce->shutdown();

    delete ce;
    return 0;
}
