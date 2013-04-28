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


namespace Caelum {

class Scene {
  public:
    Scene(const String& sceneName, const String& typeName);
    virtual ~Scene();

    // Normal functions
    const String& getName() {return mName;}
    const String& getTypeName() {return mTypeName;}

    // GameObject related functions
    GameObject* getRootObject() {return mRootObject;}
    GameObject* createGameObject(const String& name);

    // TODO Default component manager getFunctions
    RenderLayer* getRenderLayer() {return mRenderLayer;}
    //void getPhysicsLayer();
    //void getNetWorkLayer();

    Ogre::SceneManager* getSceneMgr() {return _mScene;}

  protected:
    String mName;
    String mTypeName;
    GameObject *mRootObject;

    // provisional
    RenderLayer *mRenderLayer;

  private:
    // Dereferenced opaque pointer
    Ogre::SceneManager *_mScene;
};

}

#endif // SCENE_H
