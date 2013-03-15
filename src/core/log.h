/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_CORE_LOG_H
#define SRC_CORE_LOG_H

#include "EnginePrerequisites.h"

namespace Caelum {

class Log {
  public:
    virtual ~Log() {}

    virtual const String& getName() = 0;
    virtual void logMessage(const String& message) = 0;
};

}

#endif // SRC_CORE_LOG_H
