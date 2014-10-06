#include "oceansimulator.h"

#include "render/renderlayer.h"
#include "render/rendermanager.h"
#include "math/unitconversor.h"
#include "render/ogre/oceanlistener.h"

#include "core/logmanager.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgrePlane.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/OgreMaterial.h>

#include <Hydrax/Hydrax.h>
#include <Hydrax/Noise/Perlin/Perlin.h>
#include <Hydrax/Modules/ProjectedGrid/ProjectedGrid.h>

using namespace Caelum;

OceanSimulator::OceanSimulator(const String &name, RenderLayer *layer, RealisticSky *sky)
    : FixedComponent(name) {
    mLayer = layer;
    mSky = sky->getSkyX();

    Ogre::SceneManager *sm = mLayer->_getSceneManager();
    //Ogre::Camera *cam = RenderManager::getSingleton().getRenderWindow()->getActualCamera();
    Ogre::Camera *cam = mLayer->_getSceneManager()->getCamera("Cam");
    Ogre::Viewport *vp = RenderManager::getSingleton().getRenderWindow()->getActualViewport();

    LogManager::getSingleton().logMessage("INSTANCING HYDRAX");
    mOcean = new Hydrax::Hydrax(sm, cam , vp);
    LogManager::getSingleton().logMessage("CREATING MODULE");
    Hydrax::Module::ProjectedGrid *mModule
            = new Hydrax::Module::ProjectedGrid( mOcean,
                                                 new Hydrax::Noise::Perlin(/**/),
                                                 Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
                                                 Hydrax::MaterialManager::NM_VERTEX,
                                                 Hydrax::Module::ProjectedGrid::Options(/**/));
    LogManager::getSingleton().logMessage("SETTING MODULE FOR HYDRAX");
    mOcean->setModule(static_cast<Hydrax::Module::Module*>(mModule));
    create("HydraxDemo.hdx", sky);
    LogManager::getSingleton().logMessage("PREPARING GRADIENTS");
    prepareGradients();
    LogManager::getSingleton().logMessage("EVERYTHING SET FOR HYDRAX");
}

OceanSimulator::~OceanSimulator() {
    delete mAmbientGradient;
    delete mSunGradient;
    delete mWaterGradient;
    delete mOcean;
}

void OceanSimulator::prepareGradients() {
    mWaterGradient = new SkyX::ColorGradient();
    mWaterGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.779105)*0.4, 1));
    mWaterGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.729105)*0.3, 0.8));
    mWaterGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.25, 0.6));
    mWaterGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.2, 0.5));
    mWaterGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.1, 0.45));
    mWaterGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.025, 0));
    // Sun
    mSunGradient = new SkyX::ColorGradient();
    mSunGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.5, 1.0f));
    mSunGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.4, 0.75f));
    mSunGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.3, 0.5625f));
    mSunGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.6,0.5,0.2)*1.5, 0.5f));
    mSunGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.25, 0.45f));
    mSunGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.01, 0.0f));
    // Ambient
    mAmbientGradient = new SkyX::ColorGradient();
    mAmbientGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 1.0f));
    mAmbientGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 0.6f));
    mAmbientGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.6, 0.5f));
    mAmbientGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.3, 0.45f));
    mAmbientGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.1, 0.35f));
    mAmbientGradient->addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.05, 0.0f));
}

void OceanSimulator::create(const String &configFileName, RealisticSky *sky) {
    LogManager::getSingleton().logMessage("CFG SETTING FOR HYDRAX");
    mOcean->loadCfg(configFileName);
    LogManager::getSingleton().logMessage("CREATING HYDRAX");
    if (!mOcean->isCreated())
        mOcean->create();
    LogManager::getSingleton().logMessage("RTT HYDRAX");
    mOcean->getRttManager()->addRttListener(new OceanListener(this, sky));
}

void OceanSimulator::setPosition(const Vector3 &pos) {
    mOcean->setPosition(UnitConversor::toOgre(pos));
}

void OceanSimulator::addTerrainAbsortionPass(Terrain *terrain, long x, long y) {
    Ogre::Terrain *tile = terrain->getTile(x,y);
    mOcean->getMaterialManager()->addDepthTechnique(
                static_cast<Ogre::MaterialPtr>(tile->getMaterial())->createTechnique());
}

void OceanSimulator::update(Real deltaTime) {
    if (mSky) {
        Ogre::Camera *mCamera = RenderManager::getSingleton().getRenderWindow()->getActualCamera();
        Ogre::Vector3 lightDir = -(mSky->getController()->getSunDirection());
        Real point = (-lightDir.y +1.0f) /2.0f;

        // water color
        mOcean->setWaterColor(mWaterGradient->getColor(point));

        // Sun position and color reflected in water

        Ogre::Vector3 sunPos = mCamera->getDerivedPosition() - lightDir * mSky->getMeshManager()->getSkydomeRadius(mCamera)*0.1;
        mOcean->setSunPosition(sunPos);
        Ogre::Vector3 sunCol = mSunGradient->getColor(point);
        mOcean->setSunColor(sunCol);

        //Ogre::Light *Light0 = mSceneMgr->getLight("Light0"), *Light1 = mSceneMgr->getLight("Light1");
        //Light0->setPosition(mCamera->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius()*0.02);
        //Light1->setDirection(lightDir);
        //Light0->setSpecularColour(sunCol.x, sunCol.y, sunCol.z);
        //Ogre::Vector3 ambientCol = mAmbientGradient.getColor(point);
        //Light0->setDiffuseColour(ambientCol.x, ambientCol.y, ambientCol.z);
    }
    mOcean->update(deltaTime);
}
