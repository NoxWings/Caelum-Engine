/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "gameobject.h"

#include <OGRE/OgreSceneNode.h>
#include "math/unitconversor.h"
#include "game/movablecomponent.h"

using namespace Caelum;


GameObject::GameObject(const String& name, GameObject *parentObject,
                       const Vector3 &position, const Quaternion &orientation) {
    mName = name;
    mParentObject = parentObject;
    _mNode = parentObject->_mNode->createChildSceneNode(name);
    this->move(position);
    this->rotate(orientation);
    // Optimization: pre-reserve 6 components space (4 native + 2 for custom);
    mComponents.reserve(6);
}

GameObject::GameObject(Ogre::SceneNode *node) {
    _mNode = node;
    mName = _mNode->getName();
    mParentObject = 0;
}

GameObject::~GameObject() {
    // Delete childs on cascade
    for (ObjectMap::iterator it = mChildObjects.begin();
         it != mChildObjects.end();
         it++) {
        delete (it->second);
    }
    mChildObjects.clear();

    // Delete the actual object
    if (mParentObject) {
        // destroy the actual node
        mParentObject->_mNode->removeAndDestroyChild(mName);
    }
}

GameObject* GameObject::createChildGameObject(const String &name) {
    GameObject *child = new GameObject(name, this);
    mChildObjects.insert(ObjectMap::value_type(name, child));
    return child;
}

void GameObject::destroyChildGameObject(const String &name) {
    ObjectMap::iterator it = mChildObjects.find(name);
    if (it != mChildObjects.end()) {
        // delete the object
        delete it->second;
        // remove from child map
        mChildObjects.erase(it);
    }
}

void GameObject::addChild(GameObject *child) {
    // If the child has already a parent
    // detatch from its actual parent
    GameObject *oldParent = child->getParent();
    if (oldParent) {
        oldParent->removeChild(child);
    } 

    // Add connect parent-child
    mChildObjects.insert(ObjectMap::value_type(child->getName(),child));
    child->mParentObject = this;
    // Connect the parent-child nodes
    this->_mNode->addChild(child->_mNode);
}

void GameObject::removeChild(GameObject *child) {
    // If I am its parent
    if (child->getParent() == this) {
        // Remove parent-child link
        mChildObjects.erase(child->mName);
        child->mParentObject = 0;
        // Detatch actual nodes
        this->_mNode->removeChild(child->_mNode);
    }
}

void GameObject::setParent(GameObject *parent) {
    if (mParentObject) {
        mParentObject->removeChild(this);
    }
    parent->addChild(this);
}

bool GameObject::getInheritOrientation() {
    return _mNode->getInheritOrientation();
}

bool GameObject::getInheritScale() {
    return _mNode->getInheritScale();
}

void GameObject::setInheritOrientation(bool inherit) {
    _mNode->setInheritOrientation(inherit);
}

void GameObject::setInheritScale(bool inherit) {
    _mNode->setInheritScale(inherit);
}

const Quaternion& GameObject::getOrientation() {
    static Ogre::Quaternion oq;
    oq = _mNode->getOrientation();
    UnitConversor::OgreQuaternionToQuaternion(oq, mOrientation);
    return mOrientation;
}

const Quaternion& GameObject::getDerivedOrientation() {
    static Ogre::Quaternion oq;
    oq = _mNode->_getDerivedOrientation();
    UnitConversor::OgreQuaternionToQuaternion(oq, mDerivedOri);
    return mDerivedOri;
}

void GameObject::setOrientation(const Quaternion &q) {
    _mNode->setOrientation(q.w, q.x, q.y, q.z);
}

void GameObject::setOrientation(Real w, Real x, Real y, Real z) {
    _mNode->setOrientation(w,x,y,z);
}

void GameObject::setDerivedOrientation(const Quaternion &q) {
    static Ogre::Quaternion oq;
    UnitConversor::QuaternionToOgreQuaternion(q, oq);
    _mNode->_setDerivedOrientation(oq);
}

void GameObject::resetOrientation() {
    _mNode->resetOrientation();
}

void GameObject::roll(const Radian &angle, TransformSpace ts) {
    static Ogre::Radian ograd;
    UnitConversor::RadianToOgreRadian(angle, ograd);
    _mNode->roll(ograd, Ogre::Node::TransformSpace(ts));
}

void GameObject::pitch(const Radian &angle, TransformSpace ts) {
    static Ogre::Radian ograd;
    UnitConversor::RadianToOgreRadian(angle, ograd);
    _mNode->pitch(ograd, Ogre::Node::TransformSpace(ts));
}

void GameObject::yaw(const Radian &angle, TransformSpace ts) {
    static Ogre::Radian ograd;
    UnitConversor::RadianToOgreRadian(angle, ograd);
    _mNode->yaw(ograd, Ogre::Node::TransformSpace(ts));
}

void GameObject::rotate(const Vector3 &axis, const Radian &angle, TransformSpace ts) {
    static Ogre::Vector3 ov3;
    static Ogre::Radian ograd;
    UnitConversor::Vector3ToOgreVector3(axis, ov3);
    UnitConversor::RadianToOgreRadian(angle,ograd);
    _mNode->rotate(ov3, ograd, Ogre::Node::TransformSpace(ts));
}

void GameObject::rotate(const Quaternion &q, TransformSpace ts) {
    static Ogre::Quaternion oq;
    UnitConversor::QuaternionToOgreQuaternion(q, oq);
    _mNode->rotate(oq, Ogre::Node::TransformSpace(ts));
}

const Vector3& GameObject::getPosition() {
    static Ogre::Vector3 ov3;
    ov3 = _mNode->getPosition();
    UnitConversor::OgreVector3ToVector3(ov3, mPosition);
    return mPosition;
}

const Vector3& GameObject::getDerivedPosition() {
    static Ogre::Vector3 ov3;
    ov3 = _mNode->_getDerivedPosition();
    UnitConversor::OgreVector3ToVector3(ov3, mDerivedPos);
    return mDerivedPos;
}

void GameObject::setPosition(const Vector3 &pos) {
    _mNode->setPosition(pos.x, pos.y, pos.z);
}

void GameObject::setPosition(Real x, Real y, Real z) {
    _mNode->setPosition(x, y, z);
}

void GameObject::setDerivedPosition(const Vector3 &pos) {
    static Ogre::Vector3 ov3;
    UnitConversor::Vector3ToOgreVector3(pos,ov3);
    _mNode->_setDerivedPosition(ov3);
}

void GameObject::resetPosition() {
    _mNode->setPosition(Ogre::Vector3::ZERO);
}

void GameObject::move(const Vector3& mov, TransformSpace ts) {
    _mNode->translate(mov.x, mov.y, mov.z, Ogre::Node::TransformSpace(ts));
}

void GameObject::move(Real x, Real y, Real z, TransformSpace ts) {
    _mNode->translate(x, y, z, Ogre::Node::TransformSpace(ts));
}

const Vector3& GameObject::getScale() {
    static Ogre::Vector3 oscale;
    oscale = _mNode->getScale();
    UnitConversor::OgreVector3ToVector3(oscale, mScale);
    return mScale;
}

const Vector3& GameObject::getDerivedScale() {
    static Ogre::Vector3 oscale;
    oscale = _mNode->_getDerivedScale();
    UnitConversor::OgreVector3ToVector3(oscale, mDerivedScale);
    return mDerivedScale;
}

void GameObject::setScale(const Vector3 &scale) {
    _mNode->setScale(scale.x, scale.y, scale.z);
}

void GameObject::setScale(Real x, Real y, Real z) {
    _mNode->setScale(x, y, z);
}

void GameObject::scale(Real x, Real y, Real z) {
    _mNode->scale(x, y, z);
}

void GameObject::attachComponent(MovableComponent *comp) {
    if (comp->isAttached()) {
        comp->detachFromGameObject();
    }
    mComponents.push_back(comp);
    comp->_setGameObject(this);
}

void GameObject::detachComponent(MovableComponent *comp) {
    if (comp->getGameObject() == this) {
        for (ComponentVector::iterator it = mComponents.begin();
             it != mComponents.end(); it++) {
            // Look for the same component
            if ((*it) == comp) {
                mComponents.erase(it);
                break;
            }
        }
        comp->_setGameObject(NULL);
    }
}

void GameObject::notifyPosition() {
    this->getDerivedPosition();  // update mDerivedPos
    for(ComponentVector::iterator it = mComponents.begin();
        it != mComponents.end();
        ++it) {
        (*it)->updatePosition(mDerivedPos);
    }
}

void GameObject::notifyOrientation() {
    this->getDerivedOrientation();  // update mDerivedOri
    for(ComponentVector::iterator it = mComponents.begin();
        it != mComponents.end();
        ++it) {
        (*it)->updateOrientation(mDerivedOri);
    }
}

void GameObject::notifyScale() {
    this->getDerivedScale();  // update mDerivedScale
    for(ComponentVector::iterator it = mComponents.begin();
        it != mComponents.end();
        ++it) {
        (*it)->updateScale(mDerivedScale);
    }
}
