#include "demomain.h"

#include <sstream>
#include "render/rendermanager.h"
#include "util/stringutils.h"

#include <MyGUI/MyGUI.h>
#include <MyGUI/MyGUI_Button.h>

using namespace Caelum;


DemoMain::DemoMain(const Caelum::String &sceneName)
    : Demo(sceneName) {
}

DemoMain::~DemoMain() {
}

void DemoMain::setStage(int n) {
    switch (n) {
    case 0:
        mLog->logMessage("Estableciendo paso 0");
        // Estado inicial, no hay nada
        stage0();
        break;
    default:
        if (mCurrentStage < 0) {
            mContinue = false;
        } else {
            // next demo
            switch (mIndex) {
            case 0:
                mGameMan->transitionTo("DemoIntro");
                break;
            case 1:
                mGameMan->transitionTo("DemoDesign");
                break;
            case 2:
                mGameMan->transitionTo("DemoMesh");
                break;
            case 3:
                mGameMan->transitionTo("DemoPhysics");
                break;
            case 4:
                //destroyHydrax();
                mGameMan->transitionTo("DemoAdvanced");
                break;
            default:
                break;
            }
        }
        break;
    }
}

void DemoMain::stage0() {
    mCamObj->setPosition(0, 200, 5000);
    // look up a little bit
    mCamObj->setDirection(Caelum::Vector3(0.35, 0.20, 1), Caelum::GameObject::TS_WORLD);
    mRenderLayer->setAmbientLight(Caelum::ColourValue(0.5, 0.5, 0.5));
    mRenderLayer->setBackgroundColour(Caelum::ColourValue::White);
    mSky->setPreset(Caelum::RealisticSky::SKY_DESERT);
    mSky->setSettings(Caelum::SkySettings(
                          Vector3(9.2f, 7.5f, 20.5f), // Time( current hour, sunrise hour, sunfall hour)
                          0.0f, 0.0f, // Time multiplier + moon phase
                          AtmosphereOptions(
                              9.77501f, 10.2963f,  // Inner + outer radius
                              0.01f,               // height pos
                              0.00195f, 0.000775f,  // rayleightMul + mieMul
                              30,                  // sunIntensity
                              Vector3(0.57f, 0.52f, 0.44f), // waveLenght
                              -0.991f,             // phaseFunc
                              2.0f , 4                 // exposure + number of sambles
                              ),
                          false, true, // layered clouds + volumetric clouds
                          300, false,  // vclouds windspeed + wclouds autoupdate
                          Radian(270), // vclouds wind direction
                          Vector3(0.63f,0.63f,0.7f), // Ambient colour
                          Vector4(0.35, 0.2, 0.92, 0.1), // Light response
                          Vector4(0.4, 0.7, 0, 0), // Ambient factor
                          Vector2(0.15, 0.75) // Humidity + cloud avg size
                          )
                      );

    mLog->logMessage("ESTADO INICIAL DE MAIN COMPLETO");
}

void DemoMain::selectButton(int n) {
    if (n < 0) mIndex = 4;
    if (n > 4) mIndex = 0;
    mButton->setStateSelected(false);
    String name = "Button";
    name += StringUtils::to_string(mIndex);
    mButton = mGuiInstance->getGUI()->findWidget<MyGUI::Button>("D0_"+name);
    mButton->setStateSelected(true);
}

void DemoMain::createHydrax() {
    if (!mOcean) {
        // Test ocean
        mTerrain = mRenderLayer->createTerrain("Island", 513, 4000);
        mTerrain->configureImport(500, 6, 8, 4000, 10);
        mTerrain->loadTile(0, 0, "Island.dat");
        mTerrain->loadAllTiles();
        mPhysicsLayer->createStaticTerrain(mTerrain);

        mOcean = mRenderLayer->createOcean("DemoMainOcean", "", mSky);
        mOcean->setPosition(Vector3(0, 120, 0));
        mOcean->addTerrainAbsortionPass(mTerrain, 0, 0);
    }
}

void DemoMain::destroyHydrax() {
    mRenderLayer->destroyComponent(mOcean);
    mOcean = 0;
}

void DemoMain::buttonExit(MyGUI::WidgetPtr _sender) {
    mContinue = false;
}

void DemoMain::enter() {
    Demo::enter();
    mCamMovSpeed = 100;
    mOcean = 0;

    // Shadows

    mRenderLayer->setAmbientLight(ColourValue(0, 0, 0));
    mRenderLayer->setShadowTechnique(Caelum::SHADOWTYPE_TEXTURE_MODULATIVE);
    //mRenderLayer->setShadowProjectionType(Caelum::SHADOW_PROJECTION_DEFAULT);
    mRenderLayer->setShadowTextureSettings(1024, 1); // size of texture shadow + number of shadow buffers
    mRenderLayer->setShadowFarDistance(400);

    // Create sky
    mSky = mRenderLayer->createRealisticSky("DemoMainSky");

    Light *sun = mRenderLayer->createLight("SunLight", Light::LT_DIRECTIONAL);
    mScene->getRootObject()->attachComponent(sun);
    //sun->setDiffuseColour(ColourValue::White);
    sun->setDiffuseColour(ColourValue(1, 1, 1));
    sun->setSpecularColour(ColourValue(0, 0, 0));
    sun->setDirection(Vector3(0, -0.7, -0.7));
    //sun->setDirection(Vector3(0.55, -1.5, -0.75));

    // Sound
    mAudioLayer->setVolume(0.2f);
    mMusicSource = mAudioLayer->createAudioSource("BackgroundSong", "rememberme.ogg");
    mScene->getRootObject()->attachComponent(mMusicSource);
    mMusicSource->setLoop(true);

    // GUI
    mGuiInstance->loadLayout("DemoMain", "D0_");
    resume();
    mMusicSource->play();
}

void DemoMain::exit() {
    mSky->unregisterCamera(mCam);
    Demo::exit();
}

void DemoMain::pause() {
    mMusicSource->pause();
    mGuiInstance->pause();
    Demo::pause();
}

void DemoMain::resume() {
    Demo::resume();
    mGuiInstance->resume();
    mGuiInstance->setPointerVisible(false);
    mIndex = 0;
    mButton = mGuiInstance->getGUI()->findWidget<MyGUI::Button>("D0_Button0");
    mButton->setStateSelected(true);
    //mMusicSource->play();
    mLog->logMessage("COMPLETELY RESUMED");
    createHydrax();
}

void DemoMain::preRender(const Caelum::RenderEvent &evt) {
    // Cam movement
    mCamMov.normalise();
    mCamMov *= (mCamMovSpeed * evt.timeSinceLastRender);
    mCamObj->move(mCamMov.x, 0, mCamMov.z, Caelum::GameObject::TS_LOCAL);
    mCamObj->move(0, mCamMov.y, 0, Caelum::GameObject::TS_WORLD);
    mCamObj->yaw(Caelum::Degree(mCamMouseRotateSpeed * mCamYaw * evt.timeSinceLastRender), Caelum::GameObject::TS_WORLD);
    mCamObj->pitch(Caelum::Degree(mCamMouseRotateSpeed * mCamPitch * evt.timeSinceLastRender), Caelum::GameObject::TS_LOCAL);
    mCamObj->forceUpdate();
    mCamYaw = mCamPitch = 0;
}

void DemoMain::rendering(const Caelum::RenderEvent &evt) {
}

void DemoMain::postRender(const Caelum::RenderEvent &evt) {
}

bool DemoMain::mouseMoved(const Caelum::MouseEvent &evt) {
    mCamYaw = -evt.X.rel;
    mCamPitch = -evt.Y.rel;
    MyGUI::InputManager::getInstance().injectMouseMove(evt.X.abs, evt.Y.abs, evt.Z.abs);
}

bool DemoMain::mousePressed(const Caelum::MouseEvent &evt, Caelum::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMousePress(evt.X.abs, evt.Y.abs, MyGUI::MouseButton::Enum(id));
}

bool DemoMain::mouseReleased(const Caelum::MouseEvent &evt, Caelum::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMouseRelease(evt.X.abs, evt.Y.abs, MyGUI::MouseButton::Enum(id));
}

bool DemoMain::mouseClicked(const Caelum::MouseEvent &evt, Caelum::MouseButtonID id) {
}

bool DemoMain::keyPressed(const Caelum::KeyEvent &evt) {
    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(evt.key), evt.text);
    if (evt.key == Caelum::KC_ESCAPE) { mContinue = false; }
    else if (evt.key == Caelum::KC_W) { mCamMov.z = -1;}
    else if (evt.key == Caelum::KC_S) { mCamMov.z = 1;}
    else if (evt.key == Caelum::KC_A) { mCamMov.x = -1;}
    else if (evt.key == Caelum::KC_D) { mCamMov.x = 1;}
    else if (evt.key == Caelum::KC_SPACE) { mCamMov.y = 1;}
    else if (evt.key == Caelum::KC_LCONTROL) { mCamMov.y =-1;}
}

bool DemoMain::keyReleased(const Caelum::KeyEvent &evt) {
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(evt.key));
    if (evt.key == Caelum::KC_W) { mCamMov.z = 0;}
    else if (evt.key == Caelum::KC_S) { mCamMov.z = 0;}
    else if (evt.key == Caelum::KC_A) { mCamMov.x = 0;}
    else if (evt.key == Caelum::KC_D) { mCamMov.x = 0;}
    else if (evt.key == Caelum::KC_SPACE) { mCamMov.y = 0;}
    else if (evt.key == Caelum::KC_LCONTROL) { mCamMov.y = 0;}
}

bool DemoMain::keyTap(const Caelum::KeyEvent &evt) {
}

bool DemoMain::buttonPressed(const Caelum::JoyStickEvent &arg, int button) {
    std::ostringstream convert;   // stream used for the conversion
    convert << button;      // insert the textual representation of 'Number' in the characters in the stream
    mLog->logMessage(Caelum::String("Button ")+ Caelum::String(convert.str()));

    if (button == 4) { // L1 previous
        previousStage();
    } else if (button == 5) { // R1 next
        nextStage();
    } else if (button == 6) {
        Real volume = mAudioLayer->getVolume();
        volume -= 0.1;
        if (volume < 0) volume = 0;
        mAudioLayer->setVolume(volume);
    } else if (button == 7) {
        if (!mMusicSource->isplaying()) {
            mMusicSource->play();
            mAudioLayer->setVolume(0.2);
        } else {
            Real volume = mAudioLayer->getVolume();
            volume += 0.1;
            if (volume > 1) volume = 1;
            mAudioLayer->setVolume(volume);
        }
    }
    /* else if (button == 1) {
        mSky->setSettings(Caelum::mPresets[++index]);
        mSky->setTimeMultiplier(0);
        std::ostringstream prest;
        prest << index;
        mLog->logMessage(Caelum::String("Preset ")+
                         prest.str()+
                         Caelum::String(" fijado"));
    } else if (button == 3) {
        mSky->setSettings(Caelum::mPresets[--index]);
        mSky->setTimeMultiplier(0);
        std::ostringstream prest;
        prest << index;
        mLog->logMessage(Caelum::String("Preset ")+
                         prest.str()+
                         Caelum::String(" fijado"));
    }*/
}

bool DemoMain::buttonReleased(const Caelum::JoyStickEvent &arg, int button) {
    return true;
}

bool DemoMain::axisMoved(const Caelum::JoyStickEvent &arg, int axis) {
    return true;
}

bool DemoMain::sliderMoved(const Caelum::JoyStickEvent &arg, int index) {
    return true;
}

bool DemoMain::povMoved(const Caelum::JoyStickEvent &arg, int index) {
    switch (arg.mState.mPOV[index].direction) {
    case Pov::North:
        selectButton(--mIndex);
        break;
    case Pov::South:
        selectButton(++mIndex);
        break;
    default:
        break;
    }

    return true;
}

bool DemoMain::vector3Moved(const Caelum::JoyStickEvent &arg, int index) {
    return true;
}
