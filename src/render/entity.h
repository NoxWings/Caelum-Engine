#ifndef ENTITY_H
#define ENTITY_H

#include "EnginePrerequisites.h"

#include "render/rendercomponent.h"

namespace Ogre {
class Entity;
}

namespace Caelum {

enum SkeletonAnimationBlendMode {
    /// The Entity does not have an skeleton to set the blend mode
    ANIMBLEND_ERROR = -1,
    /// Animations are applied by calculating a weighted average of all animations
    ANIMBLEND_AVERAGE = 0,
    /// Animations are applied by calculating a weighted cumulative total
    ANIMBLEND_CUMULATIVE = 1
};

class Entity : public RenderComponent {
  public:
    /** Default entity constructor */
    Entity(const String& name, const String& mesh, RenderLayer* renderlayer);
    //Entity(const String &name, RenderLayer* renderlayer, Vector3 planeNormal);
    virtual ~Entity();

    // Animation
    void setSkeletonDisplay(bool enable);
    bool getSkeletonDisplay();
    void setSkeletonBlending(SkeletonAnimationBlendMode mode);
    SkeletonAnimationBlendMode getSkeletonBlending();

    // Material
    void setShadowCast(bool enable);
    bool getShadowCast();

    Ogre::Entity* _getEntity();
  private:
    Ogre::MovableObject* _getMovableObject();

    Ogre::Entity *mEntity;
};

}

#endif // ENTITY_H
