#include "render/realisticsky.h"

#include "render/renderlayer.h"
#include "render/rendermanager.h"
#include "render/cameracomponent.h"
#include "math/unitconversor.h"
#include <OGRE/Ogre.h>
#include <SKYX/SkyX.h>

using namespace Caelum;

RealisticSky::RealisticSky(const String &name, RenderLayer *renderLayer)
    : FixedComponent(name){
    mLayer = renderLayer;

    // Sky creation
    mBasicController = new SkyX::BasicController(false);
    mSky = new SkyX::SkyX(mLayer->_getSceneManager(), mBasicController);
    mSky->create();
    mSky->setLightingMode(SkyX::SkyX::LM_LDR);

    mBasicController->setMoonPhase(0.75f);

    // Adding clouds
    mSky->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
    // Clouds optimization
    mSky->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));

    // Listeners
    Ogre::Root::getSingletonPtr()->addFrameListener(mSky);
    RenderManager::getSingletonPtr()->getRenderWindow()->getActualWindow()->addListener(mSky);

    // Time multiplier
    mSky->setTimeMultiplier(0.2);
    mSky->update(0);
}

RealisticSky::~RealisticSky() {
    LogManager::getSingleton().logMessage("SKY COMPONENT: Removing Volumetric Clouds");
    // Remove VClouds
    if (mSky->getVCloudsManager()->isCreated()) {
        mSky->getVCloudsManager()->remove();
    }
    LogManager::getSingleton().logMessage("SKY COMPONENT: Removing Regular Clouds");
    // Remove cloud layers
    if (!mSky->getCloudsManager()->getCloudLayers().empty()) {
        mSky->getCloudsManager()->removeAll();
    }
    LogManager::getSingleton().logMessage("SKY COMPONENT: Freeing resources VClouds");
    // Free SkyX Resources
    mSky->remove();
    LogManager::getSingleton().logMessage("SKY COMPONENT: unregistering window listener");
    RenderManager::getSingletonPtr()->getRenderWindow()->getActualWindow()->removeListener(mSky);
    LogManager::getSingleton().logMessage("SKY COMPONENT: unregistering frame listener");
    Ogre::Root::getSingletonPtr()->removeFrameListener(mSky);
    LogManager::getSingleton().logMessage("SKY COMPONENT: deleting skyx");
    delete mSky;
    LogManager::getSingleton().logMessage("SKY COMPONENT: deleting controller");
    delete mBasicController;
    LogManager::getSingleton().logMessage("SKY COMPONENT: Everything deleted");
}

void RealisticSky::setTimeMultiplier(Real timeMul) {
    mSky->setTimeMultiplier(timeMul);
}

Real RealisticSky::getTimeMultiplier() {
    return mSky->getTimeMultiplier();
}

void RealisticSky::setPreset(SKY_PRESET_ENUM presetIndex) {
    if (presetIndex >= 0 && presetIndex <= SKY_NUM_PRESETS) {
        this->setSettings(mPresets[presetIndex]);
    }
}


void RealisticSky::setSettings(const SkySettings& settings) {
    Ogre::Radian  rad;
    Ogre::Vector3 vec3;
    Ogre::Vector4 vec4;


    // -------------------
    // General options
    // -------------------
    mSky->setTimeMultiplier(settings.timeMultiplier);
    UnitConversor::Vector3ToOgreVector3(settings.time, vec3);
    mBasicController->setTime(vec3);
    mBasicController->setMoonPhase(settings.moonPhase);

    // -------------------
    // Atmosphere options
    // -------------------
    UnitConversor::Vector3ToOgreVector3(settings.atmosphereOpt.WaveLength, vec3);
    SkyX::AtmosphereManager::Options opts(settings.atmosphereOpt.InnerRadius,
                                          settings.atmosphereOpt.OuterRadius,
                                          settings.atmosphereOpt.HeightPosition,
                                          settings.atmosphereOpt.RayleighMultiplier,
                                          settings.atmosphereOpt.MieMultiplier,
                                          settings.atmosphereOpt.SunIntensity,
                                          vec3,
                                          settings.atmosphereOpt.G,
                                          settings.atmosphereOpt.Exposure,
                                          settings.atmosphereOpt.NumberOfSamples);
    mSky->getAtmosphereManager()->setOptions(opts);

    // -------------------
    // Layered clouds
    // -------------------
    if (settings.layeredClouds) {
        // Create layer cloud
        if (mSky->getCloudsManager()->getCloudLayers().empty()) {
            mSky->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
        }
    } else {
        // Remove layer cloud
        if (!mSky->getCloudsManager()->getCloudLayers().empty()) {
            mSky->getCloudsManager()->removeAll();
        }
    }

    // -----------------
    // Volumetric clouds
    // -----------------
    if (settings.volumetricClouds) {
        // Create VClouds
        if (!mSky->getVCloudsManager()->isCreated()) {
            // SkyX::MeshManager::getSkydomeRadius(...) works for both finite and infinite(=0) camera far clip distances
            // mSky->getVCloudsManager()->create(mSky->getMeshManager()->getSkydomeRadius(mRenderingCamera));
            mSky->getVCloudsManager()->create(mSky->getMeshManager()->getSkydomeRadius(RenderManager::getSingletonPtr()->getRenderWindow()->getActualCamera()));
        }

        mSky->getVCloudsManager()->setWindSpeed(settings.vcWindSpeed);
        mSky->getVCloudsManager()->setAutoupdate(settings.vcAutoupdate);

        SkyX::VClouds::VClouds* vclouds = mSky->getVCloudsManager()->getVClouds();

        UnitConversor::RadianToOgreRadian(settings.vcWindDir, rad);
        vclouds->setWindDirection(rad);
        UnitConversor::Vector3ToOgreVector3(settings.vcAmbientColor, vec3);
        vclouds->setAmbientColor(vec3);

        vec4.x = settings.vcLightResponse.x;
        vec4.y = settings.vcLightResponse.y;
        vec4.z = settings.vcLightResponse.z;
        vec4.w = settings.vcLightResponse.w;
        vclouds->setLightResponse(vec4);

        vec4.x = settings.vcAmbientFactors.x;
        vec4.y = settings.vcAmbientFactors.y;
        vec4.z = settings.vcAmbientFactors.z;
        vec4.w = settings.vcAmbientFactors.w;
        vclouds->setAmbientFactors(vec4);

        vclouds->setWheater(settings.vcWheater.x, settings.vcWheater.y, false);

        // -----------------
        // Lightnings
        // -----------------
        vclouds->getLightningManager()->setEnabled(settings.vcLightnings);
        vclouds->getLightningManager()->setAverageLightningApparitionTime(settings.vcLightningsAT);
        UnitConversor::Vector3ToOgreVector3(settings.vcLightningsColor, vec3);
        vclouds->getLightningManager()->setLightningColor(vec3);
        vclouds->getLightningManager()->setLightningTimeMultiplier(settings.vcLightningsTM);

    } else {
        // Remove VClouds
        if (mSky->getVCloudsManager()->isCreated()) {
            mSky->getVCloudsManager()->remove();
        }
    }

    mSky->update(0);
}

void RealisticSky::unregisterCamera(Camera *c) {
    mSky->getVCloudsManager()->getVClouds()->unregisterCamera(c->_getCamera());
}
