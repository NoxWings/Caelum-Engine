#include "cameracomponent.h"

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneManager.h>
#include "render/renderlayer.h"
#include "math/unitconversor.h"
#include "render/rendermanager.h"

using namespace Caelum;

Camera::Camera(const String& name, RenderLayer *renderlayer)
    : RenderComponent(name, renderlayer) {
    mCam = mLayer->_getSceneManager()->createCamera(name);

    // Frustum Clip
    mCam->setFarClipDistance(50000);
    mCam->setNearClipDistance(0.25);

    // Reset Position & Orientation
    mCam->setPosition(Ogre::Vector3::ZERO);
    mCam->setOrientation(Ogre::Quaternion::IDENTITY);

}

Camera::~Camera() {
    mLayer->_getSceneManager()->destroyCamera(mCam);
    mCam = 0;
}

const Radian& Camera::getFOVy() {
    static Radian fov;
    UnitConversor::OgreRadianToRadian(mCam->getFOVy(), fov);
    return fov;
}

void Camera::setFOVy(const Radian &fov) {
    static Ogre::Radian oFov;
    UnitConversor::RadianToOgreRadian(fov, oFov);
    mCam->setFOVy(oFov);
}

void Camera::setPerspectiveProjection() {
    mCam->setProjectionType(Ogre::PT_PERSPECTIVE);
}

void Camera::setOrtographicProjection() {
    mCam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
}

void Camera::setNearClipDistance(Real near) {
    mCam->setNearClipDistance(Ogre::Real(near));
}

void Camera::setFarClipDistance(Real far) {
    mCam->setFarClipDistance(Ogre::Real(far));
}

void Camera::setPolygonMode(POLYGON_MODE mode) {
    mCam->setPolygonMode(Ogre::PolygonMode(mode));
}

POLYGON_MODE Camera::getPolygonMode() {
    return POLYGON_MODE(mCam->getPolygonMode());
}

void Camera::setAsActiveCamera() {
    RenderManager::getSingletonPtr()->getRenderWindow()->setViewportCamera(this);
}

Ogre::Camera* Camera::_getCamera() {
    return mCam;
}

Ogre::MovableObject* Camera::_getMovableObject() {
    return mCam;
}
