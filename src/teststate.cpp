#include "teststate.h"

#include "game/scenemanager.h"
#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreShadowCameraSetup.h>
#include <OGRE/OgreLight.h>

#include "render/rendermanager.h"
#include <sstream>

using namespace Caelum;

void TestState::enter() {
    mContinue = true;

    mLog->logMessage("GAMEMANAGER: Creating Test State.");
    mScene = mSceneManager->createScene("TestScene","EXTERIOR_CLOSE");

    /// Camera
    camobj = mScene->createGameObject("CameraObject");
    cam = mScene->getRenderLayer()->createCamera("MainCamera");
    camobj->attachComponent(cam);
    camobj->setPosition(-115,400,300);
    //camobj->setPosition(0, 7, 12);
    camobj->setFixedYawAxis(true);
    movSpeed = 300;
    rotateSpeed = 30;
    cam->setAsActiveCamera(); // Set active camera

    /// SHADOW CONFIG
    mScene->getRenderLayer()->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
    mScene->getRenderLayer()->setShadowTechnique(Caelum::SHADOWTYPE_TEXTURE_MODULATIVE);
    mScene->getRenderLayer()->setShadowProjectionType(Caelum::SHADOW_PROJECTION_LISPSM);
    mScene->getRenderLayer()->setShadowFarDistance(400);
    mScene->getRenderLayer()->setShadowTextureSettings(1024, 1); // size of texture shadow + number of shadow buffers


    /// LIGHT
    GameObject *sunObj = mScene->createGameObject("SunObj");
    Light* sunLight = mScene->getRenderLayer()->createLight("SunLight", Light::LT_DIRECTIONAL);
    sunLight->_getActualLight()->setDirection(0.55, -1.5, -0.75);
    sunObj->attachComponent(sunLight);
    //Vector3 lightdir(0.55, -0.3, 0.75);
    //sunObj->setDirection(lightdir, GameObject::TS_WORLD);
    sunLight->setDiffuseColour(ColourValue::White);
    sunLight->setSpecularColour(ColourValue(0.4, 0.4, 0.4));

    /// ANIMATED ENTITY
    /*GameObject *charObj = mScene->createGameObject("CharObject");
    Entity *charEnt = mScene->getRenderLayer()->createEntity("CharEnt", "Sinbad.mesh");
    charObj->attachComponent(charEnt);
    AnimationState* anim = charEnt->getAnimation("Dance");
    anim->setLoop(true);
    anim->start(0.5);
    ent = charEnt;

    camobj->lookAt(charObj);*/

    /// SKY
    //mScene->getRenderLayer()->setSkyDome(true, "Examples/CloudySky", 10, 8, 400);
    Caelum::RealisticSky *sky = mScene->getRenderLayer()->createRealisticSky("Sky");
    sky->setPreset(Caelum::RealisticSky::SKY_DESERT);
    sky->setTimeMultiplier(0.5);

    /// TERRAIN
    terrain = mScene->getRenderLayer()->createTerrain("terrain", 513, 4000);
    // Initial terrain group configuration
    terrain->configureImport(500, 3, 8, 3000, 20);
    terrain->configureLight(sunLight);
    // Texture setting
    terrain->setTexture(0, 100, "dirt_grayrocky_diffusespecular.dds", "dirt_grayrocky_normalheight.dds");
    terrain->setTextureHeightBlend(0, 0, 0);
    terrain->setTexture(1, 30, "grass_green-01_diffusespecular.dds", "grass_green-01_normalheight.dds");
    terrain->setTextureHeightBlend(1, 500, 200);
    terrain->setTexture(2, 200, "growth_weirdfungus-03_diffusespecular.dds", "growth_weirdfungus-03_normalheight.dds");
    terrain->setTextureHeightBlend(2, 2500, 150);
    // Terrain tiles settings
    terrain->setTile(0,0, "terrain.png", false);
    terrain->loadAllTiles();
    // Terrain texture blending
    terrain->blendTerrain();
    mScene->getPhysicsLayer()->createStaticTerrain(terrain);

    /// SCENE OBJECTS
    // Create House
    GameObject *houseObj = mScene->createGameObject("HouseObject");
    Entity *houseEnt = mScene->getRenderLayer()->createEntity("House", "tudorhouse.mesh");
    RigidBody *housePhy = mScene->getPhysicsLayer()->createRigidBody("HousePhy", houseEnt, Vector3::UNIT_SCALE, 0, PhysicsLayer::PHY_SHAPE_TRIMESH);
    houseObj->attachComponent(houseEnt);
    houseObj->attachComponent(housePhy);
    houseObj->setPosition(-110,0,-50);
    houseObj->scale(0.085,0.085,0.085);
    houseObj->move(0,340,0);
    houseObj->notifyOrientation();
    houseObj->notifyPosition();
    houseObj->notifyScale();

    /*GameObject *dojoObj = mScene->createGameObject("DojoObject");
    Entity *dojoEnt = mScene->getRenderLayer()->createEntity("Dojo", "ts_dojo.mesh");
    RigidBody *dojoPhy = mScene->getPhysicsLayer()->createRigidBody("DojoPhy", dojoEnt, Vector3::UNIT_SCALE, 0, PhysicsLayer::PHY_SHAPE_TRIMESH);
    dojoObj->attachComponent(dojoEnt);
    dojoObj->attachComponent(dojoPhy);
    dojoObj->pitch(Degree(-90));
    dojoObj->setPosition(50, 340, -130);
    dojoObj->notifyOrientation();
    dojoObj->notifyPosition();*/

    //camobj->lookAt(dojoObj); // the camera is looking at the dojo

    camobj->lookAt(houseObj);

    axisPitch = 0.0f;
    axisYaw = 0.0f;
}

void TestState::exit() {
    mLog->logMessage("GAMEMANAGER: Exiting Test State");
    mSceneManager->destroyScene(mScene);
}

void TestState::pause() {
}

void TestState::resume() {
    cam->setAsActiveCamera();
}

bool TestState::preRenderUpdate(const Caelum::RenderEvent &evt) {
    // Movement
    mov.normalise(); // normalise the movement (uniform movement)
    mov *= (movSpeed * evt.timeSinceLastRender);
    camobj->move(mov.x, 0, mov.z, GameObject::TS_LOCAL);
    camobj->move(0, mov.y, 0, GameObject::TS_WORLD);
    // Rotation
    camobj->yaw(Degree(rotateSpeed * yawRot * evt.timeSinceLastRender), GameObject::TS_WORLD);
    camobj->pitch(Degree(rotateSpeed * pitchRot * evt.timeSinceLastRender), GameObject::TS_LOCAL);
    yawRot = 0;
    pitchRot = 0;

    camobj->yaw(Degree(rotateSpeed * axisYaw * evt.timeSinceLastRender), GameObject::TS_WORLD);
    camobj->pitch(Degree(rotateSpeed * axisPitch * evt.timeSinceLastRender), GameObject::TS_LOCAL);


    //ent->addTime(evt.timeSinceLastRender);
    mScene->getPhysicsLayer()->update(evt.timeSinceLastRender*3);
    return mContinue;
}

bool TestState::renderingUpdate(const Caelum::RenderEvent &evt) {
    return mContinue;
}
bool TestState::postRenderUpdate(const Caelum::RenderEvent &evt) {
    return mContinue;
}

bool TestState::mouseMoved(const Caelum::MouseEvent& evt) {
    yawRot = -evt.X.rel;
    pitchRot = -evt.Y.rel;
    return true;
}

bool TestState::mousePressed(const Caelum::MouseEvent& evt, Caelum::MouseButtonID id) {
    static int barrelcount = 0;       // number to be converted to a string
    static String count;          // string which will contain the result

    std::ostringstream convert;   // stream used for the conversion
    convert << barrelcount++;      // insert the textual representation of 'Number' in the characters in the stream
    count = convert.str(); // set 'Result' to the contents of the stream

    GameObject *barrelObj = mScene->createGameObject(String("barrelobj")+count);
    Entity *barrelEnt = mScene->getRenderLayer()->createEntity(String("barrel")+count, "Barrel.mesh");
    RigidBody *barrelPhy = mScene->getPhysicsLayer()->createRigidBody(String("barrelphy")+count, barrelEnt, Vector3::UNIT_SCALE, 1, PhysicsLayer::PHY_SHAPE_CONVEX);
    barrelObj->attachComponent(barrelEnt);
    barrelObj->attachComponent(barrelPhy);
    barrelObj->setPosition(camobj->getPosition());
    barrelObj->notifyPosition();
    barrelPhy->applyImpulse(camobj->getOrientation().zAxis() * -50);
    return true;
}

bool TestState::mouseReleased(const Caelum::MouseEvent& evt, Caelum::MouseButtonID id) {
    return true;
}

bool TestState::mouseClicked(const Caelum::MouseEvent& evt, Caelum::MouseButtonID id) {
    return true;
}

bool TestState::keyPressed(const Caelum::KeyEvent &evt) {
    if (evt.key == KC_ESCAPE) { mContinue = false; }
    else if (evt.key == KC_W) { mov.z = -1;}
    else if (evt.key == KC_S) { mov.z = 1;}
    else if (evt.key == KC_A) { mov.x = -1;}
    else if (evt.key == KC_D) { mov.x = 1;}
    else if (evt.key == KC_SPACE) { mov.y = 1;}
    else if (evt.key == KC_LCONTROL) { mov.y =-1;}
    else if (evt.key == KC_LSHIFT) { movSpeed*=2;}
    else if (evt.key == KC_1) {mScene->getPhysicsLayer()->setDebugMode(!mScene->getPhysicsLayer()->getDebugMode());}
    else if (evt.key == KC_2) {
        POLYGON_MODE newMode;
        switch (cam->getPolygonMode()) {
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
        cam->setPolygonMode( newMode );
    }
    return true;
}

bool TestState::keyReleased (const Caelum::KeyEvent &evt) {
    if (evt.key == KC_W) { mov.z = 0;}
    else if (evt.key == KC_S) { mov.z = 0;}
    else if (evt.key == KC_A) { mov.x = 0;}
    else if (evt.key == KC_D) { mov.x = 0;}
    else if (evt.key == KC_SPACE) { mov.y = 0;}
    else if (evt.key == KC_LCONTROL) { mov.y = 0;}
    else if (evt.key == KC_LSHIFT) { movSpeed/=2;}
    return true;
}

bool TestState::keyTap(const Caelum::KeyEvent &evt) {
    return true;
}

bool TestState::buttonPressed(const JoyStickEvent &arg, int button) {
    if (button == 7) {
        static int barrelJoyCount = 0;       // number to be converted to a string
        static String count;          // string which will contain the result

        std::ostringstream convert;   // stream used for the conversion
        convert << barrelJoyCount++;      // insert the textual representation of 'Number' in the characters in the stream
        count = convert.str(); // set 'Result' to the contents of the stream

        GameObject *barrelObj = mScene->createGameObject(String("barreljoyobj")+count);
        Entity *barrelEnt = mScene->getRenderLayer()->createEntity(String("barreljoy")+count, "Barrel.mesh");
        RigidBody *barrelPhy = mScene->getPhysicsLayer()->createRigidBody(String("barreljoyphy")+count, barrelEnt, Vector3::UNIT_SCALE, 1, PhysicsLayer::PHY_SHAPE_CONVEX);
        barrelObj->attachComponent(barrelEnt);
        barrelObj->attachComponent(barrelPhy);
        barrelObj->setPosition(camobj->getPosition());
        barrelObj->notifyPosition();
        barrelPhy->applyImpulse(camobj->getOrientation().zAxis() * -70);
    }
    return true;
}

bool TestState::buttonReleased(const JoyStickEvent &arg, int button) {
    return true;
}

bool TestState::axisMoved(const JoyStickEvent &arg, int axis) {
    // Calculate percentage
    Real percentage = Caelum::Real(arg.mState.mAxes[axis].abs) / (Caelum::Real(MAX_AXIS));
    if (percentage < 0.1f && percentage > -0.1f) percentage = 0.0f;
    // Rotate
    if (axis == 2) { // pitch
        axisPitch = 5 * -percentage;
    } else if (axis == 5) { // yaw
        axisYaw = 5 * -percentage;
    } else if (axis == 0) {
        mov.z = percentage;
    } else if (axis == 1) {
        mov.x = percentage;
    }
    return true;
}

bool TestState::sliderMoved(const JoyStickEvent &arg, int index) {
    return true;
}

bool TestState::povMoved(const JoyStickEvent &arg, int index) {
    return true;
}

bool TestState::vector3Moved(const JoyStickEvent &arg, int index) {
    return true;
}
