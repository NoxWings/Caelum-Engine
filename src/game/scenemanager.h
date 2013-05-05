/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "patterns/AssociativeCollection.h"
#include "game/scene.h"
#include "core/log.h"

namespace Caelum {

/** Fordward declaration **/
class GameEngine;

class SceneManager : public Singleton<SceneManager> {
  public:
    SceneManager();
    SceneManager(GameEngine *engine);
    ~SceneManager();

    Scene* createScene(const String& sceneName, const String& typeName);
    Scene* getScene(const String& sceneName);
    void destroyScene(const String sceneName);
    void destroyScene(Scene* scene);

  private:
    void _init(GameEngine *engine);

    GameEngine *mEngine;
    AssociativeCollection<String, Scene*> mScenes;
    Log *mLog;
};

}

#endif // SCENEMANAGER_H
