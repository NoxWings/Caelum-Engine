#include "demos/demomesh.h"

#include <sstream>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMeshManager.h>


using namespace Caelum;

DemoMesh::DemoMesh(const Caelum::String &sceneName)
    : Demo(sceneName) {
}

DemoMesh::~DemoMesh() {
}

void DemoMesh::setStage(int n) {
    switch (n) {
    case 0:
        // Estado inicial, no hay nada
        stage0();
        replaceLayout("Mesh_escena");
        break;
    case 1:
        // MESH
        // Se ve a sinbad
        stage0();
        mEnt->setVisible(true);
        replaceLayout("Mesh_capas");
        break;
    case 2:
        // TEXTURE SHADOWS
        // Sombras texturas y animación (sin blending)
        textureShadows();
        mRenderLayer->setShadowProjectionType(Caelum::SHADOW_PROJECTION_DEFAULT);
        mBlendFactor = 0;
        replaceLayout("Mesh_componentesrender");
        break;
    case 3:
        mBlendFactor = 0;
        textureShadows();
        mRenderLayer->setShadowProjectionType(Caelum::SHADOW_PROJECTION_FOCUSED);
        replaceLayout("Mesh_animacionnoblend");
        break;
    case 4:
        // LISPSM + BLENDING
        textureShadows();
        mRenderLayer->setShadowProjectionType(Caelum::SHADOW_PROJECTION_LISPSM);
        mBlendFactor = 0.5;
        replaceLayout("Mesh_animacionblend");
        break;
    case 5:
        // STENCIL
        // Sombras stencil
        stencilShadows();
        mBlendFactor = 0.5;
        replaceLayout("Mesh_stencil");
        break;
    case 6:
        textureShadows();
        mRenderLayer->setShadowProjectionType(Caelum::SHADOW_PROJECTION_DEFAULT);
        mBlendFactor = 0.5;
        replaceLayout("Mesh_texture");
        break;
    case 7:
        // SHADING
        mLog->logMessage("Estableciendo paso 5");
        stencilShadows();
        mBlendFactor = 0.5;
        changePlayer();
        replaceLayout("Mesh_material");
        break;
    default:
        if (mCurrentStage < 0) {
            // previous demo
            mGameMan->transitionBack();
        } else {
            // next demo
            mGameMan->transitionBack();
        }
        break;
    }
}

void DemoMesh::stage0() {
    // No light + White background
    mCamTarget->setPosition(mEntObj->getPosition());
    mCamObj->setPosition(0, 6, 25);
    mCamObj->lookAt(mCamTarget);
    mRenderLayer->setAmbientLight(Caelum::ColourValue::White);
    mRenderLayer->setBackgroundColour(Caelum::ColourValue::White);
    mLightObj->setPosition(5,10,5);
    // Hide all
    mEnt->setVisible(false);
    mEnt2->setVisible(false);
    mEnt2->setMaterialName(1, "Examples/Rockwall");
    mFloor->setVisible(false);
    mLight->setDiffuseColour(Caelum::ColourValue::ZERO);
    mLight->setSpecularColour(Caelum::ColourValue::ZERO);
    // Blend 0
    mBlendFactor = 0;
}

void DemoMesh::stencilShadows() {
    // Full stencil shadows
    mRenderLayer->setAmbientLight(Caelum::ColourValue(0.2, 0.2, 0.2));
    mRenderLayer->setShadowTechnique(Caelum::SHADOWTYPE_STENCIL_ADDITIVE);
    // All visible
    mEnt->setVisible(true);
    mEnt2->setVisible(false);
    mFloor->setVisible(true);
    mLight->setDiffuseColour(Caelum::ColourValue::White);
    mLight->setSpecularColour(Caelum::ColourValue(0.4, 0.4, 0.4));
    // Blend 0
    mBlendFactor = 0;
}

void DemoMesh::textureShadows() {
    // Texture shadow
    mRenderLayer->setAmbientLight(Caelum::ColourValue(0.2, 0.2, 0.2));
    mRenderLayer->setShadowTechnique(Caelum::SHADOWTYPE_TEXTURE_MODULATIVE);
    mRenderLayer->setShadowTextureSettings(1024, 1); // size of texture shadow + number of shadow buffers
    mRenderLayer->setShadowFarDistance(30);
    // All visible
    mEnt->setVisible(true);
    mEnt2->setVisible(false);
    mFloor->setVisible(true);
    mLight->setDiffuseColour(Caelum::ColourValue::White);
    mLight->setSpecularColour(Caelum::ColourValue::White);
    // Blend 0
    mBlendFactor = 0;
}

void DemoMesh::changePlayer() {
    mEnt->setVisible(false);
    mEnt2->setVisible(true);
}

void DemoMesh::replaceLayout(const String &layout) {
    if (mCurrentLayout.size() > 1) {
        mGuiInstance->unloadLayout(mCurrentLayout);
    }
    mCurrentLayout = layout;
    if (mCurrentLayout.size() > 1) {
        mGuiInstance->loadLayout(mCurrentLayout, "");
    }
}

void DemoMesh::enter() {
    Demo::enter();
    mCurrentLayout = "";
    mCamMovSpeed = 30;
    mLightRotationSpeed = 180;
    mCamRotation = mLightRotation = 0;
    // Create entities
    mEntObj = mScene->createGameObject("SinbadObj");
    mEnt = mRenderLayer->createEntity("Sinbad", "Sinbad.mesh");
    mEnt->getSkeleton()->setSkeletonBlending(ANIMBLEND_CUMULATIVE);
    mEnt2 = mRenderLayer->createEntity("Dwarf", "Sinbad.mesh");
    mEntObj->attachComponent(mEnt);
    mEntObj->attachComponent(mEnt2);
    // Create floor
    mFloorObj = mScene->createGameObject("FloorObj");
    mFloor = mRenderLayer->createEntity("Floor", "ts_dojo.mesh");
    mFloorObj->attachComponent(mFloor);
    mFloor->setShadowCast(false);
    mFloorObj->pitch(Caelum::Degree(-90));
    mFloorObj->setPosition(0,-9,0);

    // Create light
    mLightTarget = mScene->createGameObject("LightTarget");
    mLightObj = mLightTarget->createChildGameObject("LightObj");
    mLight = mRenderLayer->createLight("Light", Caelum::Light::LT_POINT);
    mLightObj->attachComponent(mLight);

    // Sound
    AudioReceptor *receptor = mAudioLayer->createAudioReceptor();
    mCamTarget->attachComponent(receptor);
    mCamTarget->notifyPosOri();
    AudioSource *source = mAudioLayer->createAudioSource("bee", "bee.ogg");
    source->setLoop(true);
    source->setVolume(1.0f);
    mLightObj->attachComponent(source);
    mLightObj->notifyPosOri();
    //source->play();

    resume();
}

void DemoMesh::exit() {
    Demo::exit();
}

void DemoMesh::pause() {
    Demo::pause();
}

void DemoMesh::resume() {
    Demo::resume();
    mGuiInstance->setPointerVisible(false);
}

void DemoMesh::preRender(const Caelum::RenderEvent &evt) {
}

void DemoMesh::rendering(const Caelum::RenderEvent &evt) {
    mCamMov.normalise();
    mCamMov *= (mCamMovSpeed * evt.timeSinceLastRender);
    mCamObj->move(mCamMov.x, 0, mCamMov.z, Caelum::GameObject::TS_LOCAL);
    mCamObj->move(0, mCamMov.y, 0, Caelum::GameObject::TS_WORLD);
    mCamObj->yaw(Caelum::Degree(mCamMouseRotateSpeed * mCamYaw * evt.timeSinceLastRender), Caelum::GameObject::TS_WORLD);
    mCamObj->pitch(Caelum::Degree(mCamMouseRotateSpeed * mCamPitch * evt.timeSinceLastRender), Caelum::GameObject::TS_LOCAL);
    mCamYaw = mCamPitch = 0;

    mLightTarget->yaw(Caelum::Degree(-mLightRotation*mLightRotationSpeed*evt.timeSinceLastRender));
    mLightTarget->notifyOrientation();
    mCamTarget->yaw(Caelum::Degree(mCamRotation*mLightRotationSpeed*evt.timeSinceLastRender));
    mCamTarget->notifyOrientation();
    //mEnt->addTime(evt.timeSinceLastRender);
}

void DemoMesh::postRender(const Caelum::RenderEvent &evt) {
}

bool DemoMesh::mouseMoved(const Caelum::MouseEvent &evt) {
    mCamYaw = -evt.X.rel;
    mCamPitch = -evt.Y.rel;
    return true;
}

bool DemoMesh::mousePressed(const Caelum::MouseEvent &evt, Caelum::MouseButtonID id) {
    return true;
}

bool DemoMesh::mouseReleased(const Caelum::MouseEvent &evt, Caelum::MouseButtonID id) {
    return true;
}

bool DemoMesh::mouseClicked(const Caelum::MouseEvent &evt, Caelum::MouseButtonID id) {
    return true;
}

bool DemoMesh::keyPressed(const Caelum::KeyEvent &evt) {
    if (evt.key == Caelum::KC_ESCAPE) { mContinue = false; }
    else if (evt.key == Caelum::KC_W) { mCamMov.z = -1;}
    else if (evt.key == Caelum::KC_S) { mCamMov.z = 1;}
    else if (evt.key == Caelum::KC_A) { mCamMov.x = -1;}
    else if (evt.key == Caelum::KC_D) { mCamMov.x = 1;}
    else if (evt.key == Caelum::KC_SPACE) { mCamMov.y = 1;}
    else if (evt.key == Caelum::KC_LCONTROL) { mCamMov.y =-1;}
    return true;
}

bool DemoMesh::keyReleased(const Caelum::KeyEvent &evt) {
    if (evt.key == Caelum::KC_W) { mCamMov.z = 0;}
    else if (evt.key == Caelum::KC_S) { mCamMov.z = 0;}
    else if (evt.key == Caelum::KC_A) { mCamMov.x = 0;}
    else if (evt.key == Caelum::KC_D) { mCamMov.x = 0;}
    else if (evt.key == Caelum::KC_SPACE) { mCamMov.y = 0;}
    else if (evt.key == Caelum::KC_LCONTROL) { mCamMov.y = 0;}
    return true;
}

bool DemoMesh::keyTap(const Caelum::KeyEvent &evt) {
    return true;
}

bool DemoMesh::buttonPressed(const Caelum::JoyStickEvent &arg, int button) {
    if (button == 4) { // previous
        previousStage();
    } else if (button == 5) { // next
        nextStage();
    } else if (button == 6) {
        POLYGON_MODE newMode;
        switch (mCam->getPolygonMode()) {
        case PM_POINTS:
            newMode = PM_WIREFRAME;
            break;
        case PM_WIREFRAME:
            newMode = PM_SOLID;
            break;
        case PM_SOLID:
            newMode = PM_POINTS;
            break;
        default:
            newMode = PM_SOLID;
        }
        mCam->setPolygonMode( newMode );
    } else if (button == 7) {
        bool display = mEnt->getSkeleton()->getSkeletonDisplay();
        mEnt->getSkeleton()->setSkeletonDisplay(!display);
    }
    return true;
}

bool DemoMesh::buttonReleased(const Caelum::JoyStickEvent &arg, int button) {
    return true;
}

bool DemoMesh::axisMoved(const Caelum::JoyStickEvent &arg, int axis) {
    int moved = arg.mState.mAxes[axis].abs;
    float percentageMoved = 0.0f;
    // Percentage abs moved
    if (moved > 0) {
        percentageMoved = Real(moved) / Real(Caelum::MAX_AXIS);
    } else if (moved < 0) {
        percentageMoved = Real(moved) / Real(Caelum::MIN_AXIS);
        percentageMoved = -percentageMoved;
    }
    // Clamp
    if (percentageMoved < 0.2f && percentageMoved > -0.2f) {
        percentageMoved = 0.0f;
    }

    if (axis == 1) {
        mLightRotation = percentageMoved;
    } else if (axis == 5) {
        mCamRotation = percentageMoved;
    }

    return true;
}

bool DemoMesh::sliderMoved(const Caelum::JoyStickEvent &arg, int index) {
    return true;
}

bool DemoMesh::povMoved(const Caelum::JoyStickEvent &arg, int index) {
    String topAnimationName = "", baseAnimationName = "";
    bool changeTop = false, changeBase = false;
    switch (arg.mState.mPOV[index].direction) {
    case Pov::North:
        topAnimationName = "RunTop";
        changeTop = true;
        break;
    case Pov::South:
        baseAnimationName = "RunBase";
        changeBase = true;
        break;
    case Pov::West:
        baseAnimationName = "Dance";
        changeBase = true;
        break;
    case Pov::East:
        topAnimationName = "IdleTop";
        baseAnimationName = "IdleBase";
        changeTop = changeBase = true;
        break;
    default:
        break;
    }

    // enable / disable
    if (changeTop) {
        if (mEnt->getSkeleton()->getAnimation(topAnimationName)->getEnabled()) {
            mEnt->stopAnimation(topAnimationName, mBlendFactor);
        } else {
            mEnt->startAnimation(topAnimationName, mBlendFactor);
        }
    }
    if (changeBase) {
        if (mEnt->getSkeleton()->getAnimation(baseAnimationName)->getEnabled()) {
            mEnt->stopAnimation(baseAnimationName, mBlendFactor);
        } else {
            mEnt->startAnimation(baseAnimationName, mBlendFactor);
        }
    }
    return true;
}

bool DemoMesh::vector3Moved(const Caelum::JoyStickEvent &arg, int index) {
    return true;
}
