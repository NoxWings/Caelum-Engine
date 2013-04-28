#include "entity.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include "render/renderlayer.h"

using namespace Caelum;

Entity::Entity(const String& name, const String &mesh, RenderLayer* renderlayer)
    : RenderComponent(name, renderlayer) {
    mEntity = mLayer->_getSceneManager()->createEntity(name,mesh);
}

Entity::~Entity() {
    mLayer->_getSceneManager()->destroyEntity(mEntity);
}

Ogre::MovableObject* Entity::_getMovableObject() {
    return mEntity;
}
