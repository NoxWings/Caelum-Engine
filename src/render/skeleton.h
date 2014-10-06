#ifndef SKELETON_H
#define SKELETON_H

#include "EnginePrerequisites.h"

#include <list>
#include <map>

namespace Ogre {
class Entity;
class SkeletonInstance;
class AnimationStateSet;
class AnimationState;
}

namespace Caelum {

class Entity;
class AnimationState;

enum SkeletonAnimationBlendMode {
    /// The Entity does not have an skeleton to set the blend mode
    ANIMBLEND_ERROR = -1,
    /// Animations are applied by calculating a weighted average of all animations
    ANIMBLEND_AVERAGE = 0,
    /// Animations are applied by calculating a weighted cumulative total
    ANIMBLEND_CUMULATIVE = 1
};

class Skeleton {
  public:
    Skeleton(Entity* entity);
    ~Skeleton();

    void addTime(Real deltaTime);

    //void startAnimation(unsigned int animationId, Real fadeInTime = 0);
    void startAnimation(const String& animationName, Real fadeInTime = 0);
    //void stopAnimation(unsigned int animationId, Real fadeOutTime = 0);
    void stopAnimation(const String& animationName, Real fadeOutTime = 0);
    bool hasAnimation(const String& name);
    //AnimationState* getAnimation(unsigned int animationId);
    AnimationState* getAnimation(const String& name);

    void setSkeletonDisplay(bool enable);
    bool getSkeletonDisplay();
    void setSkeletonBlending(SkeletonAnimationBlendMode mode);
    SkeletonAnimationBlendMode getSkeletonBlending();


    unsigned int getNumAnimations();
    unsigned int getNumEnabledAnimations();

    void _notifyEnabled(AnimationState* animationState, bool enabled);

    //void getEnabledAnimations();
    //void getAllAnimations();

  private:
    Ogre::Entity* mEntity;
    Ogre::SkeletonInstance* mSkeleton;

    std::map<String, AnimationState*> mAnimationStateMap;
    std::list<AnimationState*> mEnabledAnimationStateList;
    std::list<AnimationState*>::iterator mEnabledAnimationListIterator;
};

class AnimationState {
  public:
    AnimationState(Skeleton* skeleton, Ogre::AnimationState* state);
    ~AnimationState();

    void addTime(Real deltaTime);
    void start(Real fadeInTime = 0);
    void stop(Real fadeOutTime = 0);
    bool hasEnded();

    bool getEnabled();
    void setEnabled(bool enabled);

    bool getLoop();
    void setLoop(bool loop);

    const String& getName();

    Real getLength();
    Real getTime();
    void setTime(Real time);

    Real getWeight();
    void setWeight(Real weight);

    Real getSpeed();
    void setSpeed(Real speed);

  private:
    Ogre::AnimationState *mState;
    Skeleton *mSkeleton;
    /** @remarks
     * This is the incremental weight needed for the next frame
     * Its value could be positive or negative, indicating fadeIn
     * or fadeOut respectively.
     * deltaWeight = 1 / fadeTime; (supposing time it's expressed in seconds)
     */
    Real mDeltaWeight;
    Real mUpdatedWeight;
    Real mAnimationSpeed;
};

} // namespace Skeleton

#endif // SKELETON_H
