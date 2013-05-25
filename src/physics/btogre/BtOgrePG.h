/*
 * =====================================================================================
 *
 *       Filename:  BtOgrePG.h
 *
 *    Description:  The part of BtOgre that handles information transfer from Bullet to
 *                  Ogre (like updating graphics object positions).
 *
 *        Version:  1.0
 *        Created:  27/12/2008 03:40:56 AM
 *
 *         Author:  Nikhilesh (nikki)
 *
 * =====================================================================================
 */

#ifndef _BtOgreGP_H_
#define _BtOgreGP_H_

#include <bullet/btBulletDynamicsCommon.h>
#include <OGRE/OgreSceneNode.h>
#include "game/gameobject.h"
#include "math/unitconversor.h"
#include "BtOgreExtras.h"

namespace BtOgre {

//A MotionState is Bullet's way of informing you about updates to an object.
//Pass this MotionState to a btRigidBody to have your SceneNode updated automaticaly.
class RigidBodyState : public btMotionState
{
    protected:
        btTransform mTransform;
        btTransform mCenterOfMassOffset;

        Caelum::GameObject *mObject;
        Ogre::SceneNode *mNode;

    public:
        RigidBodyState() {
            this->setGameObject(NULL);
        }

        RigidBodyState(Caelum::GameObject *object) {
            this->setGameObject(object);
        }

        virtual void getWorldTransform(btTransform &ret) const {
            ret = mTransform;
        }

        virtual void setWorldTransform(const btTransform &in) {
            if (mObject == NULL)
                return;

            mTransform = in;
            btTransform transform = in * mCenterOfMassOffset;

            btQuaternion rot = transform.getRotation();
            btVector3 pos = transform.getOrigin();

            // Using the node directly is much more efficient than using the object
            mNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
            mNode->setPosition(pos.x(), pos.y(), pos.z());

            // TODO notify changes to gameobject childs
            //mObject->notifyPosition();
            //mObject->notifyOrientation();
        }

        void setGameObject(Caelum::GameObject *object) {
            mObject = object;
            if (object != NULL) {
                mNode = mObject->getNode();
                mTransform = btTransform(Caelum::UnitConversor::toBullet(mObject->getOrientation()),
                                         Caelum::UnitConversor::toBullet(mObject->getPosition()));
            } else {
                mNode = NULL;
                mTransform = btTransform(btQuaternion::getIdentity(), btVector3(0,0,0));
            }
            mCenterOfMassOffset = btTransform::getIdentity();
        }
};

//Softbody-Ogre connection goes here!

}

#endif
