/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_MOTIONCONTROLLER_H_
#define SRC_INPUT_MOTIONCONTROLLER_H_

#include "EnginePrerequisites.h"

namespace Caelum {

class MotionController {
  public:
    MotionController();
    virtual ~MotionController() {}

    virtual void capture() = 0;
};
}  // namespace NAGE

#endif  // SRC_INPUT_MOTIONCONTROLLER_H_
