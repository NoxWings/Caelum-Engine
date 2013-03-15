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
    // initialise engine
    ce->setup(false);

    // shutdown engine
    ce->shutdown();
    delete ce;
    return 0;
}
