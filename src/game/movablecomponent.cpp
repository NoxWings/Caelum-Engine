#include "movablecomponent.h"

using namespace Caelum;

MovableComponent::MovableComponent(const String& name) : Component(name), mGameObject(0) {
}

MovableComponent::~MovableComponent() {
}

void MovableComponent::attachTo(GameObject *obj) {
    obj->attachComponent(this);
}

void MovableComponent::detachFromGameObject() {
    if (mGameObject != 0) {
        mGameObject->detachComponent(this);
    }
}

void MovableComponent::_setGameObject(GameObject *obj) {
    mGameObject = obj;
}
