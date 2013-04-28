#include "rendercomponent.h"

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreMovableObject.h>

using namespace Caelum;

RenderComponent::RenderComponent(const String& name, RenderLayer* renderLayer)
    : MovableComponent(name), mLayer(renderLayer) {
}

RenderComponent::~RenderComponent() {
}

void RenderComponent::_setGameObject(GameObject *obj) {
    if (mGameObject) {
        // If we are attached detach from current parent
        mGameObject->getNode()->detachObject(this->_getMovableObject());
        mGameObject = NULL;
    }
    if (obj) {
        // If the new parent is valid, attach to it
        mGameObject = obj;
        mGameObject->getNode()->attachObject(this->_getMovableObject());
    }
}
