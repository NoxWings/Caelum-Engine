#ifndef ENTITY_H
#define ENTITY_H

#include "EnginePrerequisites.h"

#include "render/rendercomponent.h"
#include "render/skeleton.h"

namespace Ogre {
class Entity;
}

namespace Caelum {

class Entity : public RenderComponent {
  public:
    /** Default entity constructor */
    Entity(const String& name, const String& mesh, RenderLayer* renderlayer);
    //Entity(const String &name, RenderLayer* renderlayer, Vector3 planeNormal);
    virtual ~Entity();

    // Animation
    bool hasSkeleton();
    Skeleton* getSkeleton();

    void addTime(Real deltaTime);
    void startAnimation(const String& animationName, Real fadeInTime = 0);
    void stopAnimation(const String& animationName, Real fadeOutTime = 0);
    bool hasAnimation(const String& animationName);
    AnimationState* getAnimation(const String& animationName);

    // Material
    void setMaterialName(const String& materialName);
    void setMaterialName(unsigned int index, const String& materialName);
    unsigned int getNumMaterials();
    void setVisible(bool visible);
    void setShadowCast(bool enable);
    bool getShadowCast();

    // Component
    void update(Real deltaTime);

    Ogre::Entity* _getEntity();
  private:
    Ogre::MovableObject* _getMovableObject();

    Ogre::Entity *mEntity;

    Skeleton *mSkeleton;
};

}

#endif // ENTITY_H
