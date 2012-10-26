/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_MOTIONCONTROLLERFACTORY_H_
#define SRC_INPUT_MOTIONCONTROLLERFACTORY_H_

#include "EnginePrerequisites.h"

#include "motioncontroller.h"
#include "patterns/AbstractFactory.h"
#include <list>


namespace Caelum {

class MotionControllerFactory : public AbstractFactory<MotionController> {
  public:
    MotionControllerFactory();
    ~MotionControllerFactory();

    virtual MotionController* createInstance(const std::string& name);
    virtual void destroyInstance(MotionController* instance);

  private:
    std::list<MotionController*> controllers;
};

}  // namespace NAGE

#endif  // SRC_INPUT_MOTIONCONTROLLERFACTORY_H_
