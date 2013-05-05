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

void Entity::setSkeletonDisplay(bool enable) {
    mEntity->setDisplaySkeleton(enable);
}

bool Entity::getSkeletonDisplay() {
    return mEntity->getDisplaySkeleton();
}

void Entity::setSkeletonBlending(SkeletonAnimationBlendMode mode) {
    if (mEntity->hasSkeleton()) {
        mEntity->getSkeleton()->setBlendMode(Ogre::SkeletonAnimationBlendMode(mode));
    }
}

SkeletonAnimationBlendMode Entity::getSkeletonBlending() {
    if (mEntity->hasSkeleton()) {
        Ogre::SkeletonAnimationBlendMode mode = mEntity->getSkeleton()->getBlendMode();
        return SkeletonAnimationBlendMode(mode);
    }
    return ANIMBLEND_ERROR;
}

void Entity::setShadowCast(bool enable) {
    mEntity->setCastShadows(enable);
}

bool Entity::getShadowCast() {
    return mEntity->getCastShadows();
}

Ogre::MovableObject* Entity::_getMovableObject() {
    return mEntity;
}
