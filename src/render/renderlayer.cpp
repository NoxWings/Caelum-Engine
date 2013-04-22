#include "renderlayer.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneManager.h>

using namespace Caelum;

RenderLayer::RenderLayer(const String &name, const String typeName)
    : GameLayer(name, typeName) {
    Ogre::Root *root = Ogre::Root::getSingletonPtr();

    // TODO use the actual typeName for optimization
    _mScene = root->createSceneManager(Ogre::ST_EXTERIOR_FAR, name);
}

RenderLayer::~RenderLayer() {
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    root->destroySceneManager(_mScene);
}

Component* RenderLayer::createComponentByTypeName(const String &name, const String &typeName) {
    // TODO
    Component *comp = NULL;

    if (comp) this->addComponent(comp);
    return comp;
}

void RenderLayer::update(Real deltaTime) {
}
