#include "renderlayer.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreShadowCameraSetup.h>
#include <OGRE/OgreShadowCameraSetupPlaneOptimal.h>
#include <OGRE/OgreShadowCameraSetupFocused.h>
#include <OGRE/OgreShadowCameraSetupLiSPSM.h>
#include <OGRE/OgreShadowCameraSetupPSSM.h>

#include "render/rendermanager.h"
#include "core/logmanager.h"

#include "math/unitconversor.h"

using namespace Caelum;

RenderLayer::RenderLayer(const String &name, const String typeName)
    : GameLayer(name, typeName) {
    Ogre::Root *root = Ogre::Root::getSingletonPtr();

    // TODO use the actual typeName for optimization
    _mScene = root->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, name);
    mAmbientColour = ColourValue::White;
    mShadowColour = ColourValue(0.25f, 0.25f, 0.25f);
    _mShadowCamera = NULL;
    this->setShadowProjectionType(SHADOW_PROJECTION_DEFAULT);
}

RenderLayer::~RenderLayer() {
    LogManager::getSingleton().logMessage("***Destroying RenderLayer");
    this->destroyAllComponents();
    //delete _mShadowCamera;
    LogManager::getSingleton().logMessage("***Destroyed RenderLayer components");
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    root->destroySceneManager(_mScene);
    LogManager::getSingleton().logMessage("***Destroyed scene");
}

Component* RenderLayer::createComponentByTypeName(const String &name, const String &typeName) {
    // TODO
    Component *comp = NULL;

    if (comp) this->addComponent(comp);
    return comp;
}

Camera* RenderLayer::createCamera(const String &name) {
    Camera* camera = new Camera(name, this);
    this->addComponent(camera);
    return camera;
}

Light* RenderLayer::createLight(const String &name, Light::LightType type) {
    Light* light = new Light(name, this, type);
    this->addComponent(light);
    return light;
}

Entity* RenderLayer::createEntity(const String &name, const String &mesh) {
    Entity *ent = new Entity(name, mesh, this);
    this->addComponent(ent);
    return ent;
}

Terrain* RenderLayer::createTerrain(const String &name, uint16 terrainTileSize, Real terrainWorldSize, Vector3 origin) {
    Terrain *terrain = new Terrain(name, this, terrainTileSize, terrainWorldSize, origin);
    this->addComponent(terrain);
    return terrain;
}

RealisticSky* RenderLayer::createRealisticSky(const String &name) {
    RealisticSky *sky = new RealisticSky(name, this);
    this->addComponent(sky);
    return sky;
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

OceanSimulator* RenderLayer::createOcean(const String &name, const String &configFile, RealisticSky* sky = NULL) {
    OceanSimulator* ocean = new OceanSimulator(name, this, sky);
    this->addComponent(ocean);
    return ocean;
}

void RenderLayer::setBackgroundColour(const ColourValue &colour) {
    static Ogre::ColourValue oc;
    oc.r = colour.r; oc.g = colour.g; oc.b = colour.b; oc.a = colour.a;
    Caelum::RenderManager::getSingletonPtr()->getRenderWindow()->getActualViewport()->setBackgroundColour(oc);
}

void RenderLayer::setAmbientLight(const ColourValue &colour) {
    static Ogre::ColourValue oc;
    mAmbientColour = colour;
    oc.r = colour.r; oc.g = colour.g; oc.b = colour.b; oc.a = colour.a;
    _mScene->setAmbientLight(oc);
}

const ColourValue& RenderLayer::getAmbientLight() {
    return mAmbientColour;
}

void RenderLayer::setShadowTechnique(ShadowTechnique technique) {
    _mScene->setShadowTechnique(Ogre::ShadowTechnique(technique));
}

ShadowTechnique RenderLayer::getShadowTechnique() {
    return ShadowTechnique(_mScene->getShadowTechnique());
}

void RenderLayer::setShadowColour(const ColourValue &colour) {
    static Ogre::ColourValue oc;
    mShadowColour = colour;
    oc.r = colour.r; oc.g = colour.g; oc.b = colour.b; oc.a = colour.a;
    _mScene->setShadowColour(oc);
}

const ColourValue& RenderLayer::getShadowColour() {
    return mShadowColour;
}

void RenderLayer::setShadowDirectionalLightExtrusionDistance(Real dist) {
    _mScene->setShadowDirectionalLightExtrusionDistance(dist);
}

const Real RenderLayer::getShadowDirectionalLightExtrusionDistance() {
    return _mScene->getShadowDirectionalLightExtrusionDistance();
}

void RenderLayer::setShadowFarDistance(Real distance) {
    _mScene->setShadowFarDistance(distance);
}

const Real RenderLayer::getShadowFarDistance() {
    return _mScene->getShadowFarDistance();
}

void RenderLayer::setShadowTextureSettings(unsigned short size, unsigned short count) {
    _mScene->setShadowTextureSettings(size, count);
}

void RenderLayer::setShadowCasterRenderBackFaces(bool bf) {
    _mScene->setShadowCasterRenderBackFaces(bf);
}

const bool RenderLayer::getShadowCasterRenderBackFaces() {
    return _mScene->getShadowCasterRenderBackFaces();
}

void RenderLayer::setShadowProjectionType(ShadowProjectionType type) {
    // Create & set the new shadow camera setup
    mShadowCameraType = type;
    switch (mShadowCameraType) {
    case SHADOW_PROJECTION_DEFAULT:
        _mShadowCamera = new Ogre::DefaultShadowCameraSetup();
        break;
    case SHADOW_PROJECTION_FOCUSED:
        _mShadowCamera = new Ogre::FocusedShadowCameraSetup();
        break;
    case SHADOW_PROJECTION_LISPSM:
        _mShadowCamera = new Ogre::LiSPSMShadowCameraSetup();
        break;
    case SHADOW_PROJECTION_PSSM:
        _mShadowCamera = new Ogre::PSSMShadowCameraSetup();
        break;
    default:
        _mShadowCamera = new Ogre::DefaultShadowCameraSetup();
    }
    _mScene->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(_mShadowCamera));
    // Note: The smart pointer will delete the newly created object
}

const ShadowProjectionType RenderLayer::getShadowProjectionType() {
    return mShadowCameraType;
}

void RenderLayer::update(Real deltaTime) {
    for (AssociativeCollection<String, Component*>::ItemMap::iterator it = mComponents.mItems.begin();
         it != mComponents.mItems.end();
         ++it) {
        it->second->update(deltaTime);
    }
}
