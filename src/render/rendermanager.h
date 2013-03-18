#ifndef SRC_RENDER_RENDERMANAGER_H_
#define SRC_RENDER_RENDERMANAGER_H_

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "core/pluginloader.h"
#include "core/log.h"

namespace Caelum {

/** Forward declarations **/
class GameEngine;

class RenderManager : public Singleton<RenderManager>, public PluginLoader {
  public:
    RenderManager();
    ~RenderManager();

    /// Render
    bool setRenderSystem(const String& renderSystem);

    /// Window Events
    void registerWindowEventListener();
    void unregisterWindowEventListener();

    /// PluginLoader interface
    void loadPlugin(const String& pluginPath);
    void unloadPlugin(const String& pluginPath);

  private:
    void createRenderWindow();
    void destroyRenderWindow();

    GameEngine* mEngine;
    Log* mLog;
};

}

#endif // SRC_RENDER_RENDERMANAGER_H_
