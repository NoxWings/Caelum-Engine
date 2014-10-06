#ifndef LIGHT_H
#define LIGHT_H

#include "EnginePrerequisites.h"

#include "render/rendercomponent.h"
#include "render/colourvalue.h"

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

    void setDiffuseColour(const ColourValue& diffuse);
    void setSpecularColour(const ColourValue& specular);

    void setType(LightType lt);
    const LightType getType();

    void setDirection(const Vector3& dir);
    const Vector3 &getDirection();

    Ogre::Light* _getActualLight() { return mLight;}

  private:
    Ogre::MovableObject* _getMovableObject();
    Ogre::Light* mLight;
    LightType mLightType;
    Vector3 mDirection;
};

}

#endif // LIGHT_H
