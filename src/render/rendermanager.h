/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_RENDER_RENDERMANAGER_H_
#define SRC_RENDER_RENDERMANAGER_H_

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "core/pluginloader.h"
#include "core/log.h"

#include "render/renderwindow.h"
#include "render/windowlistener.h"
#include "render/renderlistener.h"
#include "game/layermanager.h"
#include "render/renderlayer.h"
#include "patterns/SimpleCollection.h"

namespace Caelum {

/** Forward declarations **/
class GameEngine;

class RenderManager :
        public Singleton<RenderManager>,
        public PluginLoader,
        public LayerManager,
        public SimpleCollection<RenderListener*> {
  public:
    RenderManager();
    virtual ~RenderManager();

    /// Render
    bool setRenderSystem(const String& renderSystem);

    /// Rendering Loop & Events
    virtual void startRendering() = 0;

    void addRenderListener(RenderListener* listener);
    void removeRenderListener(RenderListener* listener);

    bool isRenderLoopActive() { return mIsRenderLoopActive;}

    /// Window Events
    void addWindowEventListener(WindowListener* listener);
    void removeWindowEventListener(WindowListener* listener);

    /// PluginLoader interface
    void loadPlugin(const String& pluginPath);
    void unloadPlugin(const String& pluginPath);

    /// Render Layer Manager
    GameLayer* createLayer(const String& name, const String& typeName);
    RenderLayer* createRenderLayer(const String& name, const String& typeName);

  protected:
    void createRenderWindow();
    void destroyRenderWindow();

    GameEngine* mEngine;
    Log* mLog;
    RenderWindow* mWindow;
    bool mIsRenderLoopActive;
};

}

#endif // SRC_RENDER_RENDERMANAGER_H_
