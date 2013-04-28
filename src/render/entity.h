#ifndef ENTITY_H
#define ENTITY_H

#include "EnginePrerequisites.h"

#include "render/rendercomponent.h"

namespace Ogre {
class Entity;
}

namespace Caelum {

class Entity : public RenderComponent {
  public:
    Entity(const String& name, const String& mesh,RenderLayer* renderlayer);
    virtual ~Entity();

  private:
    Ogre::MovableObject* _getMovableObject();

    Ogre::Entity *mEntity;
};

}

#endif // ENTITY_H
