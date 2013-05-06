#include "light.h"

#include "render/renderlayer.h"
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreMovableObject.h>
#include <OGRE/OgreLight.h>

namespace Caelum {

Light::Light(const String &name, RenderLayer *renderlayer, LightType type)
    : RenderComponent(name, renderlayer) {
    mLight = mLayer->_getSceneManager()->createLight(name);
    this->setType(type);
}

Light::~Light() {
    mLayer->_getSceneManager()->destroyLight(mLight);
}

void Light::setDiffuseColour(const ColourValue &diffuse) {
    mLight->setDiffuseColour(diffuse.r, diffuse.g, diffuse.b);
}

void Light::setSpecularColour(const ColourValue &specular) {
    mLight->setSpecularColour(specular.r, specular.g, specular.b);
}

void Light::setType(LightType lt) {
    mLightType = lt;
    mLight->setType(Ogre::Light::LightTypes(lt));
}

const Light::LightType Light::getType() {
    return mLightType;
}

Ogre::MovableObject* Light::_getMovableObject() {
    return mLight;
}

}  // namespace Caelum
