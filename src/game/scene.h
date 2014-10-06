/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SCENE_H
#define SCENE_H

#include "EnginePrerequisites.h"
#include "game/gameobject.h"
#include "render/renderlayer.h"
#include "physics/physicslayer.h"
#include "audio/audiolayer.h"
#include "core/log.h"

#include <map>


namespace Caelum {

class Scene {
  public:
    Scene(const String& sceneName, const String& typeName);
    virtual ~Scene();

    // Normal functions
    const String& getName() {return mName;}
    const String& getTypeName() {return mTypeName;}

    // GameObject related functions
    GameObject* findObject(const String &name);
    GameObject* getRootObject() {return mRootObject;}
    GameObject* createGameObject(const String& name);

    // TODO Default component manager getFunctions
    RenderLayer* getRenderLayer() {return mRenderLayer;}
    PhysicsLayer* getPhysicsLayer() {return mPhysicsLayer;}
    AudioLayer* getAudioLayer() {return mAudioLayer;}
    //void getNetWorkLayer();

    Ogre::SceneManager* getSceneMgr() {return _mScene;}
    Ogre::SceneManager* addObject();

    friend class GameObject;
  protected:
    void notifyCreated(GameObject *obj);
    void notifyDeleted(GameObject *obj);

    String mName;
    String mTypeName;
    GameObject *mRootObject;
    std::map<String, GameObject*> mObjectMap;

    // provisional
    RenderLayer *mRenderLayer;
    PhysicsLayer *mPhysicsLayer;
    AudioLayer *mAudioLayer;

  private:
    // Dereferenced opaque pointer
    Ogre::SceneManager *_mScene;
    Log *mLog;
};

}

#endif // SCENE_H
