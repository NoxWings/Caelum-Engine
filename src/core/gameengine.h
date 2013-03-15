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
#include "render/rendermanager.h"


namespace Caelum {

class GameEngine : public Singleton<GameEngine> {
  public:
    GameEngine();
    ~GameEngine();

    /// SETUP & SHUTDOWN
    void setup(bool initialiseResources = true);
    void shutdown();

    /// GETTERS SUBSYSTEM
    LogManager*       getLogManager() { return mLogManager;}
    PluginManager*    getPluginManager() { return mPluginMan;}
    ResourceManager*  getResourcemanager() { return mResourceMan;}

  private:
    /// CREATE SUBSYSTEM
    void createLogManager();
    void createPluginManager();
    void createMainSystem();
    void createResourceManager();
    void createRenderManager();
    void createInputManager();

    /// DESTROY SUBSYSTEM
    void destroyLogManager();
    void destroyPluginManager();
    void destroyMainSystem();
    void destroyResourceManager();
    void destroyRenderManager();
    void destroyInputManager();

    /// ATRIBUTES
    LogManager *mLogManager;
    PluginManager *mPluginMan;
    ResourceManager *mResourceMan;
    RenderManager *mRenderMan;
    //InputManager *mInputMan;
};

}

#endif // SRC_CORE_GAMEENGINE_H_
