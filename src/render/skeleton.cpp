#include "render/skeleton.h"

#include "render/entity.h"
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSkeleton.h>
#include <OGRE/OgreSkeletonInstance.h>
#include <OGRE/OgreAnimationState.h>

using namespace Caelum;

/* SKELETON */
Skeleton::Skeleton(Entity* entity) {
    // 0.- Ensure the real entity has an skeleton
    assert(entity->_getEntity()->hasSkeleton());
    // 1.- Get OGRE animation classes
    mEntity = entity->_getEntity();
    mSkeleton = mEntity->getSkeleton();
    Ogre::AnimationStateSet* animationStateSet;
    animationStateSet = mEntity->getAllAnimationStates();
    mEnabledAnimationStateList.clear();

    // 2.- Populate map with our own animation states
    Ogre::AnimationStateIterator it = animationStateSet->getAnimationStateIterator();
    String animationName;
    AnimationState* animationState;
    Ogre::AnimationState* ogreAnimationState;

    while (it.hasMoreElements()) {
        // Return an ogre animation state
        animationName = it.peekNextKey(); // returns the key, without advancing
        ogreAnimationState = it.getNext(); // returns the animationstate and advances to the next item
        // Wrap our own animation state
        animationState = new AnimationState(this, ogreAnimationState);
        mAnimationStateMap[animationName] = animationState;
    }
}

Skeleton::~Skeleton() {
    mEnabledAnimationStateList.clear();
    for (std::map<String, AnimationState*>::iterator it = mAnimationStateMap.begin();
         it != mAnimationStateMap.end();
         it++) {
        // delete our animation states wrappers
        delete (it->second);
    }
}

void Skeleton::addTime(Real deltaTime) {
    for ( mEnabledAnimationListIterator = mEnabledAnimationStateList.begin();
          mEnabledAnimationListIterator != mEnabledAnimationStateList.end();
          mEnabledAnimationListIterator++ ) {
        // Add time to every enabled animation
        (*mEnabledAnimationListIterator)->addTime(deltaTime);
    }
}

/*void Skeleton::startAnimation(unsigned int animationId, Real fadeInTime) {
    // 1.- Get the animation
    Caelum::AnimationState *state = this->getAnimation(animationId);
    // 2.- Start the animation
    if (state != NULL) {
        state->start(fadeInTime);
    } else {
        // Error log msg
    }
}*/

void Skeleton::startAnimation(const String &animationName, Real fadeInTime) {
    // 1.- Get the animation
    Caelum::AnimationState *state = this->getAnimation(animationName);
    // 2.- Start the animation
    if (state != NULL) {
        state->start(fadeInTime);
    } else {
        // Error log msg
    }
}

/*void Skeleton::stopAnimation(unsigned int animationId, Real fadeOutTime) {
    // 1.- Get the animation
    Caelum::AnimationState *state = this->getAnimation(animationId);
    // 2.- Stop the animation
    if (state != NULL) {
        state->stop(fadeOutTime);
    } else {
        // Error log msg
    }
}*/

void Skeleton::stopAnimation(const String &animationName, Real fadeOutTime) {
    // 1.- Get the animation
    Caelum::AnimationState *state = this->getAnimation(animationName);
    // 2.- Stop the animation
    if (state != NULL) {
        state->stop(fadeOutTime);
    } else {
        // Error log msg
    }
}

bool Skeleton::hasAnimation(const String &name) {
    return mEntity->hasAnimationState(name);
}

/*Caelum::AnimationState* Skeleton::getAnimation(unsigned int animationId) {
    // TODO
}*/

AnimationState* Skeleton::getAnimation(const String &name) {
    std::map<String, AnimationState*>::iterator it = mAnimationStateMap.find(name);
    if ( it == mAnimationStateMap.end() ) return NULL;
    return (it->second);
}

void Skeleton::setSkeletonDisplay(bool enable) {
    mEntity->setDisplaySkeleton(enable);
}

bool Skeleton::getSkeletonDisplay() {
    return mEntity->getDisplaySkeleton();
}

void Skeleton::setSkeletonBlending(SkeletonAnimationBlendMode mode) {
    mSkeleton->setBlendMode(Ogre::SkeletonAnimationBlendMode(mode));
}

SkeletonAnimationBlendMode Skeleton::getSkeletonBlending() {
    Ogre::SkeletonAnimationBlendMode mode = mEntity->getSkeleton()->getBlendMode();
    return SkeletonAnimationBlendMode(mode);
}


unsigned int Skeleton::getNumAnimations() {
    return mAnimationStateMap.size();
}

unsigned int Skeleton::getNumEnabledAnimations() {
    return mEnabledAnimationStateList.size();
}

void Skeleton::_notifyEnabled(AnimationState *animationState, bool enabled) {
    // removing it anyways because any already enabled animation can be re-enabled
    // but we don't want to add it twice to the list.
    mEnabledAnimationStateList.remove(animationState);
    if (enabled) {
        mEnabledAnimationStateList.push_back(animationState);
    }
}


/* ANIMATIONSTATE */
AnimationState::AnimationState(Skeleton* skeleton, Ogre::AnimationState* state)
    : mDeltaWeight(0), mUpdatedWeight(0), mAnimationSpeed(1) {
    mSkeleton = skeleton;
    mState = state;
}

AnimationState::~AnimationState() {
    mState = 0;
    mSkeleton = 0;
}

void AnimationState::addTime(Real deltaTime) {
    if (mDeltaWeight != 0) {
        // Calculate new weight += (deltaWeight * time)
        mUpdatedWeight = this->getWeight() + (mDeltaWeight * deltaTime);
        // FadeIn limit reached
        if (mUpdatedWeight >= 1 && mDeltaWeight >= 0) {
            mDeltaWeight = 0; // Stop fading
            mUpdatedWeight = 1; // Clamp
        // FadeOut limit reached
        } else if (mUpdatedWeight <= 0 && mDeltaWeight <= 0) {
            mDeltaWeight = 0; // Stop fading
            mUpdatedWeight = 0; // Clamp
            this->setEnabled(false); // Disable animation
        }
        this->setWeight(mUpdatedWeight); // Update bone weight
    }
    // Small optimization (I'd rather using a conditional than a product(times 1) every frame
    mState->addTime((mAnimationSpeed=1)?deltaTime:deltaTime * mAnimationSpeed);
}

void AnimationState::start(Real fadeInTime) {
    // Initial state
    this->setEnabled(true); // Enable Animation
    this->setTime(0); // start time position

    // Calculate deltaWeight (should remain possitive)
    if (fadeInTime != 0) {
        //mDeltaWeight = Real(1) / ((fadeInTime > 0)? fadeInTime : -fadeInTime);
        mDeltaWeight = 1 / ((fadeInTime > 0)? fadeInTime : -fadeInTime);
        this->setWeight(0); // 0% bone infuence
    } else {
        mDeltaWeight = 0;
        this->setWeight(1); // 100% bone infuence
    }
}

void AnimationState::stop(Real fadeOutTime) {
    // Calculate deltaWeight (should remain negative)
    if (fadeOutTime != 0) {
        mDeltaWeight = 1 / ((fadeOutTime < 0)? fadeOutTime : -fadeOutTime);
    } else {
        mDeltaWeight = 0;
        this->setEnabled(false); // Disable animation
    }
}

bool AnimationState::hasEnded() {
    return mState->hasEnded();
}

bool AnimationState::getEnabled() {
    return mState->getEnabled();
}

void AnimationState::setEnabled(bool enabled) {
    mState->setEnabled(enabled);
    mSkeleton->_notifyEnabled(this, enabled);
}

bool AnimationState::getLoop() {
    return mState->getLoop();
}

void AnimationState::setLoop(bool loop) {
    mState->setLoop(loop);
}

const String &AnimationState::getName() {
    return mState->getAnimationName();
}

Real AnimationState::getLength() {
    return mState->getLength();
}

Real AnimationState::getTime() {
    return mState->getTimePosition();
}

void AnimationState::setTime(Real time) {
    mState->setTimePosition(time);
}

Real AnimationState::getWeight() {
    return mState->getWeight();
}

void AnimationState::setWeight(Real weight) {
    mState->setWeight(weight);
}

Real AnimationState::getSpeed() {
    return mAnimationSpeed;
}

void AnimationState::setSpeed(Real speed) {
    mAnimationSpeed = speed;
}


