/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "EnginePrerequisites.h"

#include "math/quaternion.h"
#include "math/vector3.h"
#include <map>
#include <vector>

// Forward declaration of ogrescenemanager
namespace Ogre {
class SceneNode;
}

namespace Caelum {

/// Fordward declarations
class MovableComponent;

class GameObject {
  public:
    enum TransformSpace {
        TS_LOCAL,
        TS_PARENT,
        TS_WORLD
    };
    typedef std::map<String, GameObject*> ObjectMap;
    typedef std::vector<MovableComponent*> ComponentVector;

  public:
    GameObject(const String& name, GameObject *parentObject,
               const Vector3 &position = Vector3::ZERO, const Quaternion &orientation = Quaternion::IDENTITY); // normal constructor
    GameObject(Ogre::SceneNode *node); // direct constructor for the root gameobject creation
    virtual ~GameObject();

    const String& getName() {return mName;}
    GameObject* getParent() {return mParentObject;}

    // Child Creation & Destruction
    GameObject* createChildGameObject(const String& name);
    void destroyChildGameObject(const String& name);
    // Parent-Child relationship link control
    void addChild(GameObject* child);
    void removeChild(GameObject* child);
    void setParent(GameObject *parent);

    // Property inheritance
    bool getInheritOrientation();
    bool getInheritScale();
    void setInheritOrientation(bool inherit);
    void setInheritScale(bool inherit);

    // Orientation & rotation
    const Quaternion& getOrientation();
    const Quaternion& getDerivedOrientation();
    void setOrientation(const Quaternion &q); // TS_PARENT
    void setOrientation(Real w, Real x, Real y, Real z); // TS_PARENT
    void setDerivedOrientation(const Quaternion &q); // TS_WORLD
    void resetOrientation();

    void roll(const Radian &angle, TransformSpace ts = TS_LOCAL);
    void pitch(const Radian &angle, TransformSpace ts = TS_LOCAL);
    void yaw(const Radian &angle, TransformSpace ts = TS_LOCAL);
    void rotate(const Vector3 &axis, const Radian &angle, TransformSpace ts = TS_LOCAL);
    void rotate(const Quaternion &q, TransformSpace ts = TS_LOCAL);


    // Position & movement (default transform space is TS_PARENT)
    const Vector3& getPosition();
    const Vector3& getDerivedPosition();
    void setPosition(const Vector3 &pos); // relative to TS_PARENT
    void setPosition(Real x, Real y, Real z); // relative to TS_PARENT
    void setDerivedPosition(const Vector3 &pos); // relative to TS_WORLD
    void resetPosition();

    void move(const Vector3& mov, TransformSpace ts = TS_PARENT);
    void move(Real x, Real y, Real z, TransformSpace ts = TS_PARENT);
    //void move(const Matrix3 &axes, const Vector3 &mov, TransformSpace ts);
    //void move(const Matrix3 &axes, Real x, Real y, Real z, TransformSpace ts);

    // Scale
    const Vector3& getScale();
    const Vector3& getDerivedScale();
    void setScale(const Vector3 &scale); // relative to TS_PARENT
    void setScale(Real x, Real y, Real z); // relative to TS_PARENT

    void scale(Real x, Real y, Real z);

    // Component link
    void attachComponent(MovableComponent *comp);
    void detachComponent(MovableComponent *comp);
    void notifyPosition();
    void notifyOrientation();
    void notifyScale();
    Ogre::SceneNode* getNode() {return _mNode;}

  protected:
    String mName;
    Vector3 mPosition, mScale, mDerivedPos, mDerivedScale;
    Quaternion mOrientation, mDerivedOri;

    // GameObject hierarchy
    GameObject *mParentObject;
    ObjectMap   mChildObjects;

    // Component container
    ComponentVector mComponents;
  private:
    Ogre::SceneNode *_mNode;

};

}

#endif // GAMEOBJECT_H

