#include "cameracomponent.h"

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneManager.h>
#include "render/renderlayer.h"
#include "math/unitconversor.h"
#include "render/rendermanager.h"

using namespace Caelum;

CameraComponent::CameraComponent(const String& name, RenderLayer *renderlayer)
    : RenderComponent(name, renderlayer) {
    mCam = mLayer->_getSceneManager()->createCamera(name);

    // Frustum Clip
    mCam->setFarClipDistance(300000);
    mCam->setNearClipDistance(0.25);

    // Reset Position & Orientation
    mCam->setPosition(Ogre::Vector3::ZERO);
    mCam->setOrientation(Ogre::Quaternion::IDENTITY);

}

CameraComponent::~CameraComponent() {
    mLayer->_getSceneManager()->destroyCamera(mCam);
    mCam = 0;
}

const Radian& CameraComponent::getFOVy() {
    static Radian fov;
    UnitConversor::OgreRadianToRadian(mCam->getFOVy(), fov);
    return fov;
}

void CameraComponent::setFOVy(const Radian &fov) {
    static Ogre::Radian oFov;
    UnitConversor::RadianToOgreRadian(fov, oFov);
    mCam->setFOVy(oFov);
}

void CameraComponent::setPerspectiveProjection() {
    mCam->setProjectionType(Ogre::PT_PERSPECTIVE);
}

void CameraComponent::setOrtographicProjection() {
    mCam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
}

void CameraComponent::setNearClipDistance(Real near) {
    mCam->setNearClipDistance(near);
}

void CameraComponent::setFarClipDistance(Real far) {
    mCam->setFarClipDistance(far);
}

void CameraComponent::setAsActiveCamera() {
    RenderManager::getSingletonPtr()->getRenderWindow()->setViewportCamera(this);
}

Ogre::MovableObject* CameraComponent::_getMovableObject() {
    return mCam;
}

Ogre::Camera* CameraComponent::_getCamera() {
    return mCam;
}
