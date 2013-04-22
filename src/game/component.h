/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef COMPONENT_H
#define COMPONENT_H

#include "EnginePrerequisites.h"

namespace Caelum {

class Component {
  public:
    Component() {mName = "";}
    Component(const String& name) {mName = name;}
    virtual ~Component() {}

    const String& getName() {return mName;}

  protected:
    String mName;
};

}

#endif // COMPONENT_H
