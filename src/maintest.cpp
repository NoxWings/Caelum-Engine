/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum. */

#include <enet/enet.h>

#include "core/gameengine.h"
#include "demos/demomain.h"
#include "demos/demomesh.h"
#include "demos/demointro.h"
#include "demos/demophysics.h"
#include "demos/demoadvanced.h"
#include "demos/demonetserver.h"
#include "demos/demonetclient.h"
#include "teststate.h"

using namespace Caelum;

int main() {
    GameEngine *ce = new GameEngine();

    if (enet_initialize() != 0) {
        ce->getLogManager()->logMessage("ENET FAILED");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    DemoNetClient *demoNetClient = new DemoNetClient("DemoNetClient");
    ce->setup();
    ce->getGameManager()->addState("DemoMain", demoNetClient, true);
    ce->getGameManager()->start();
    ce->shutdown();
    delete demoNetClient;

    enet_deinitialize();

    delete ce;
    return 0;
}
