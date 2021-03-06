/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_CORE_GAMEENGINE_H_
#define SRC_CORE_GAMEENGINE_H_

#include "EnginePrerequisites.h"
#include "patterns/Singleton.h"

#include "core/logmanager.h"
#include "core/pluginmanager.h"
#include "core/resourcemanager.h"
#include "core/preferencemanager.h"
#include "render/rendermanager.h"
#include "input/inputmanager.h"
#include "game/gamemanager.h"
#include "physics/physicsmanager.h"
#include "audio/audiomanager.h"

namespace Caelum {

class GameEngine : public Singleton<GameEngine> {
  public:
    GameEngine();
    ~GameEngine();

    /// SETUP & SHUTDOWN
    void setup();
    void shutdown();

    /// GETTERS SUBSYSTEM
    LogManager*        getLogManager()        { return mLogManager;}
    PluginManager*     getPluginManager()     { return mPluginMan;}
    ResourceManager*   getResourceManager()   { return mResourceMan;}
    PreferenceManager* getPreferenceManager() { return mPreferenceMan;}
    RenderManager*     getRenderManager()     { return mRenderMan;}
    InputManager*      getInputManager()      { return mInputMan;}
    PhysicsManager*    getPhysicsManager()    { return mPhyManager;}
    AudioManager*      getAudioManager()      { return mAudioManager;}
    GameManager*       getGameManager()       { return mGameMan;}

  private:
    /// CREATE SUBSYSTEM
    void createLogManager();
    void createPluginManager();
    void createMainSystem();
    void createResourceManager();
    void createPreferenceManager();
    void createRenderManager();
    void createInputManager();
    void createPhysicsManager();
    void createAudioManager();
    void createGameManager();

    /// DESTROY SUBSYSTEM
    void destroyLogManager();
    void destroyPluginManager();
    void destroyMainSystem();
    void destroyResourceManager();
    void destroyPreferenceManager();
    void destroyRenderManager();
    void destroyInputManager();
    void destroyPhysicsManager();
    void destroyAudioManager();
    void destroyGameManager();

    /// ATRIBUTES
    LogManager *mLogManager;
    PluginManager *mPluginMan;
    ResourceManager *mResourceMan;
    PreferenceManager *mPreferenceMan;

    RenderManager *mRenderMan;
    InputManager *mInputMan;
    PhysicsManager *mPhyManager;
    AudioManager *mAudioManager;
    GameManager *mGameMan;
};

}

#endif // SRC_CORE_GAMEENGINE_H_
