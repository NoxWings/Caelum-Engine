#ifndef MOVABLECOMPONENT_H
#define MOVABLECOMPONENT_H

#include "EnginePrerequisites.h"

#include "game/component.h"

namespace Caelum {

class MovableComponent : public Component {
  public:
    MovableComponent();
    virtual ~MovableComponent();
};

}

#endif // MOVABLECOMPONENT_H
