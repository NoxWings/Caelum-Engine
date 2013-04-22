#ifndef FIXEDCOMPONENT_H
#define FIXEDCOMPONENT_H

#include "EnginePrerequisites.h"

#include "game/component.h"

namespace Caelum {

class FixedComponent : public Component {
  public:
    FixedComponent();
    virtual ~FixedComponent();
};

}

#endif // FIXEDCOMPONENT_H
