/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_GAME_EVENTLISTENER_H_
#define SRC_GAME_EVENTLISTENER_H_

#include "EnginePrerequisites.h"

#include "render/renderlistener.h"
#include "render/windowlistener.h"

namespace Caelum {

class EventListener :
        public RenderListener,
        public WindowListener {
  public:
    virtual ~EventListener() {}
};

}

#endif // SRC_GAME_EVENTLISTENER_H_
