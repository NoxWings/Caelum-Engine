
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
#include "demos/demointro.h"
#include "demos/demodesign.h"
#include "demos/demomesh.h"
#include "demos/demophysics.h"
#include "demos/demoadvanced.h"

using namespace Caelum;

int main() {
    GameEngine *ce = new GameEngine();

    DemoMain *demoMain = new DemoMain("DemoMain");
    DemoIntro *demoIntro = new DemoIntro("DemoIntro");
    DemoDesign *demoDesign = new DemoDesign("DemoDesign");
    DemoMesh *demoMesh = new DemoMesh("DemoMesh");
    DemoPhysics *demoPhysics = new DemoPhysics("DemoPhysics");
    DemoAdvanced *demoAdvanced = new DemoAdvanced("DemoMain");

    // [OPT] You can set manual resources here through resourcemanager
    ce->setup();
    // Initialize custom resources manually
    ce->getGameManager()->addState("DemoMain", demoMain, true);
    ce->getGameManager()->addState("DemoIntro", demoIntro);
    ce->getGameManager()->addState("DemoDesign", demoDesign);
    ce->getGameManager()->addState("DemoMesh", demoMesh);
    ce->getGameManager()->addState("DemoPhysics", demoPhysics);
    ce->getGameManager()->addState("DemoAdvanced", demoAdvanced);

    ce->getGameManager()->start();
    ce->shutdown();

    delete demoAdvanced;
    delete demoPhysics;
    delete demoMesh;
    delete demoDesign;
    delete demoIntro;
    delete demoMain;


    delete ce;
    return 0;
}

