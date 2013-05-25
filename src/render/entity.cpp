#include "entity.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgrePlane.h>

#include "math/unitconversor.h"
#include "render/renderlayer.h"

using namespace Caelum;

Entity::Entity(const String& name, const String &mesh, RenderLayer* renderlayer)
    : RenderComponent(name, renderlayer) {
    mEntity = mLayer->_getSceneManager()->createEntity(name,mesh);
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
    mLayer->_getSceneManager()->destroyEntity(mEntity);
}

void Entity::setSkeletonDisplay(bool enable) {
    mEntity->setDisplaySkeleton(enable);
}

bool Entity::getSkeletonDisplay() {
    return mEntity->getDisplaySkeleton();
}

void Entity::setSkeletonBlending(SkeletonAnimationBlendMode mode) {
    if (mEntity->hasSkeleton()) {
        mEntity->getSkeleton()->setBlendMode(Ogre::SkeletonAnimationBlendMode(mode));
    }
}

SkeletonAnimationBlendMode Entity::getSkeletonBlending() {
    if (mEntity->hasSkeleton()) {
        Ogre::SkeletonAnimationBlendMode mode = mEntity->getSkeleton()->getBlendMode();
        return SkeletonAnimationBlendMode(mode);
    }
    return ANIMBLEND_ERROR;
}

void Entity::setShadowCast(bool enable) {
    mEntity->setCastShadows(enable);
}

bool Entity::getShadowCast() {
    return mEntity->getCastShadows();
}

Ogre::MovableObject* Entity::_getMovableObject() {
    return mEntity;
}

Ogre::Entity* Entity::_getEntity() {
    return mEntity;
}
