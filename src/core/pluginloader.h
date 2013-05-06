/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_CORE_PLUGINLOADER_H_
#define SRC_CORE_PLUGINLOADER_H_

#include "EnginePrerequisites.h"
#include "util/stringutils.h"

namespace Caelum {

/** PluginLoader
 * @brief The PluginLoader class, provides an interface to implement
 * to create different plugin loaders.
 * @note every plugin loader is responsible for allocating, keep tracking
 * of its plugins and deallocating them on delete
 */
class PluginLoader {
  public:
    virtual ~PluginLoader() {mPluginsLoaded.clear();}

    virtual void loadPlugin(const String& pluginPath) = 0;
    virtual void unloadPlugin(const String& pluginPath) = 0;

    virtual bool isPluginLoaded(const String& pluginPath) {
        for (StringList::iterator it = mPluginsLoaded.begin();
             it != mPluginsLoaded.end();
             it++) {
            String name = *it;
            if (StringUtils::equals(pluginPath, name)) {
                return true;
            }
        }
        return false;
    }


    void registerSelfPlugin(const String& pluginPath) {
        mPluginsLoaded.push_back(pluginPath);
    }

    void unregisterSelfPlugin(const String& pluginPath) {
        bool found = false;
        for (StringList::iterator it = mPluginsLoaded.begin();
             (it != mPluginsLoaded.end()) && (!found);
             it++) {
            String name = *it;
            if (StringUtils::equals(pluginPath, name)) {
                mPluginsLoaded.erase(it);
                found = true;
            }
        }
    }

    void unloadAllPlugins() {

        while (!mPluginsLoaded.empty()) {
            // reverse unloading
            String plugin = *(mPluginsLoaded.rbegin());
            mPluginsLoaded.pop_back();
            unloadPlugin(plugin);
        }
    }

  private:
  StringList mPluginsLoaded;
};

}

#endif // SRC_CORE_PLUGINLOADER_H_
