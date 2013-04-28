#include "renderlayer.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneManager.h>
#include "math/unitconversor.h"

using namespace Caelum;

RenderLayer::RenderLayer(const String &name, const String typeName)
    : GameLayer(name, typeName) {
    Ogre::Root *root = Ogre::Root::getSingletonPtr();

    // TODO use the actual typeName for optimization
    _mScene = root->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, name);
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

CameraComponent* RenderLayer::createCamera(const String &name) {
    CameraComponent* comp = new CameraComponent(name, this);
    this->addComponent(comp);
    return comp;
}

Entity* RenderLayer::createEntity(const String &name, const String &mesh) {
    Entity *ent = new Entity(name, mesh, this);
    this->addComponent(ent);
    return ent;
}


void RenderLayer::setSkyBox(bool enable, const String& materialName, Real distance,
                            bool drawFirst, const Quaternion &orientation, const String& groupName) {
    static Ogre::Quaternion ogreOr;
    UnitConversor::QuaternionToOgreQuaternion(orientation, ogreOr);
    _mScene->setSkyBox(enable, materialName, distance, drawFirst, ogreOr, groupName);
}

void RenderLayer::setSkyDome(bool enable, const String& materialName,
                             Real curvature, Real tiling, Real distance,
                             bool drawFirst, const Quaternion& orientation,
                             int xsegments, int ysegments, int ysegments_keep,
                             const String& groupName) {
    static Ogre::Quaternion ogreOr;
    UnitConversor::QuaternionToOgreQuaternion(orientation, ogreOr);
    _mScene->setSkyDome(enable, materialName, curvature, tiling, distance, drawFirst,
                        ogreOr, xsegments, ysegments, ysegments_keep);
}

void RenderLayer::update(Real deltaTime) {
}
