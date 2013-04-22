/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum. */


#include "core/gameengine.h"
#include "teststate.h"
#include "core/logmanager.h"

#include "math/math.h"

using namespace Caelum;

int main() {
    GameEngine *ce = new GameEngine();
    TestState *testState = new TestState();

    // You can set manual resources here through resourcemanager
    ce->setup();
    // Initialize custom resources manually
    ce->getGameManager()->addState("TestState", testState, true);
    ce->getGameManager()->start();
    ce->shutdown();

    delete testState;
    delete ce;
    return 0;
}
