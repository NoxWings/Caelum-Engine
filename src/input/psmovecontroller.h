/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_PSMOVECONTROLLER_H_
#define SRC_INPUT_PSMOVECONTROLLER_H_

#include "EnginePrerequisites.h"

#include <psmoveapi/psmove.h>

namespace Caelum {

class PSMoveController {
  public:
    PSMoveController();
    ~PSMoveController();

    bool connect();
    void disconnect();

    bool isConnected();

    const char* getSerial();

  private:
    PSMove *mMove;
    PSMove_Connection_Type mCtype;
};
}  // namespace Caelum

#endif  // SRC_INPUT_PSMOVECONTROLLER_H_
