/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_CORE_PLUGINMANAGER_H_
#define SRC_CORE_PLUGINMANAGER_H_

#include "EnginePrerequisites.h"
#include "patterns/Singleton.h"
#include "core/pluginloader.h"
#include <map>

namespace Caelum {

class PluginManager : public Singleton<PluginManager> {
  public:
    PluginManager(const String& pluginFolder = "");
    ~PluginManager();

    /// Plugin loaders
    bool registerPluginLoader(const String& groupName, PluginLoader* loader);
    bool unregisterPluginLoader(const String& groupName);
    bool unregisterPluginLoader(PluginLoader* loader);

    PluginLoader* getPluginLoader(const String& groupName);

    /// Plugin folder
    void  setPluginFolder(const String& folder);
    const String& getPluginFolder();

    /// Manual plugin load
    bool loadPlugin(const String& group, const String& pluginPath);
    bool unloadPlugin(const String& group, const String& pluginPath);

    bool isPluginLoaded(const String& pluginPath);

    void loadPluginsFromFile(const String& pluginsFileName);

  private:
    typedef std::map<String, PluginLoader*> PluginLoaders;
    typedef std::multimap<String, String> PluginQueue;

    PluginLoaders mPluginLoaders;
    PluginQueue mPluginQueue;
    String mPluginFolder;
};

}

#endif // SRC_CORE_PLUGINMANAGER_H_
