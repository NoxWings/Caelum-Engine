#ifndef FIXEDCOMPONENT_H
#define FIXEDCOMPONENT_H

#include "EnginePrerequisites.h"

#include "game/component.h"

namespace Caelum {

class FixedComponent : public Component {
  public:
    FixedComponent(const String& name);
    virtual ~FixedComponent();
};

}

#endif // FIXEDCOMPONENT_H
