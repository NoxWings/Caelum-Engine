#ifndef LIGHT_H
#define LIGHT_H

#include "EnginePrerequisites.h"

#include "render/rendercomponent.h"

namespace Ogre {
class Light;
}

namespace Caelum {



class Light : public RenderComponent {
  public:
    enum LightType {
        /// Point light sources give off light equally in all directions, so require only position not direction
        LT_POINT = 0,
        /// Directional lights simulate parallel light beams from a distant source, hence have direction but no position
        LT_DIRECTIONAL = 1,
        /// Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff
        LT_SPOTLIGHT = 2
    };

  public:
    Light(const String& name, RenderLayer* renderlayer, LightType type = LT_POINT);
    virtual ~Light();

    void setType(LightType lt);
    const LightType getType();

  private:
    Ogre::MovableObject* _getMovableObject();
    Ogre::Light* mLight;
    LightType mLightType;
};

}

#endif // LIGHT_H
