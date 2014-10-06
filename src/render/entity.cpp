#include "entity.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgrePlane.h>

#include "math/unitconversor.h"
#include "render/renderlayer.h"

using namespace Caelum;

Entity::Entity(const String& name, const String &mesh, RenderLayer* renderlayer)
    : RenderComponent(name, renderlayer) {
    mEntity = mLayer->_getSceneManager()->createEntity(name,mesh);
    if (mEntity->hasSkeleton()) {
        mSkeleton = new Skeleton(this);
    } else {
        mSkeleton = NULL;
    }
}

/*Entity::Entity(const String &name, RenderLayer *renderlayer, Vector3 planeNormal) {
    Ogre::Vector3 ov3;
    UnitConversor::Vector3ToOgreVector3(planeNormal, ov3);
    Ogre::Plane plane(ov3, 0);

    Ogre::MeshManager::getSingleton().createPlane(name+"Mesh",
                                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            plane,150,150,20,20,true,1,5,5,Vector3::UNIT_Z);

    //const String& name, const String& groupName, const Plane& plane,
    //Real width, Real height,
    //int xsegments = 1, int ysegments = 1,
    //bool normals = true, unsigned short numTexCoordSets = 1,
    //Real uTile = 1.0f, Real vTile = 1.0f, const Vector3& upVector = Vector3::UNIT_Y,
    //HardwareBuffer::Usage vertexBufferUsage = HardwareBuffer::HBU_STATIC_WRITE_ONLY,
    //HardwareBuffer::Usage indexBufferUsage = HardwareBuffer::HBU_STATIC_WRITE_ONLY,
    //bool vertexShadowBuffer = true, bool indexShadowBuffer = true

    // create anentity with that plane
    Entity *ent = mLayer->_getSceneManager()->createEntity(name, name+"mesh");
    ent->setMaterialName("Examples/GrassFloor");
}*/

Entity::~Entity() {
    if (mSkeleton) delete mSkeleton;
    mLayer->_getSceneManager()->destroyEntity(mEntity);
}

bool Entity::hasSkeleton() {
    return (mSkeleton != NULL);
}

Skeleton* Entity::getSkeleton() {
    return mSkeleton;
}

void Entity::addTime(Real deltaTime) {
    if (mSkeleton)
        mSkeleton->addTime(deltaTime);
}

void Entity::startAnimation(const String &animationName, Real fadeInTime) {
    if (mSkeleton)
        mSkeleton->startAnimation(animationName, fadeInTime);
}

void Entity::stopAnimation(const String &animationName, Real fadeOutTime) {
    if (mSkeleton)
        mSkeleton->stopAnimation(animationName, fadeOutTime);
}

bool Entity::hasAnimation(const String &animationName) {
    if (mSkeleton)
        return mSkeleton->hasAnimation(animationName);
    return false;
}

AnimationState* Entity::getAnimation(const String &animationName) {
    if (mSkeleton)
        return mSkeleton->getAnimation(animationName);
    return NULL;
}

void Entity::setMaterialName(const String &materialName) {
    mEntity->setMaterialName(materialName);
}

void Entity::setMaterialName(unsigned int index, const String &materialName) {
    if (index < getNumMaterials())
        mEntity->getSubEntity(index)->setMaterialName(materialName);
}

unsigned int Entity::getNumMaterials() {
    return mEntity->getNumSubEntities();
}

void Entity::setVisible(bool visible) {
    mEntity->setVisible(visible);
}

void Entity::setShadowCast(bool enable) {
    mEntity->setCastShadows(enable);
}

bool Entity::getShadowCast() {
    return mEntity->getCastShadows();
}

void Entity::update(Real deltaTime) {
    this->addTime(deltaTime);
}

Ogre::MovableObject* Entity::_getMovableObject() {
    return mEntity;
}

Ogre::Entity* Entity::_getEntity() {
    return mEntity;
}
