/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "core/pluginmanager.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreConfigFile.h>

#include "core/logmanager.h"

using namespace Caelum;

PluginManager::PluginManager(const String &pluginFolder) {
    setPluginFolder(pluginFolder);
}

PluginManager::~PluginManager() {
    mPluginQueue.clear();
}

bool PluginManager::registerPluginLoader(const String& groupName, PluginLoader* loader) {
    // search if the groupName still available
    if ( !getPluginLoader(groupName) ) {
        // load the plugin loader
        mPluginLoaders.insert(
                    std::pair<String, PluginLoader*>(
                        groupName.c_str(),
                        loader));
        // look for queued plugins
        PluginQueue::iterator it = mPluginQueue.find(groupName);
        while (it != mPluginQueue.end()) {
            loader->loadPlugin(it->second);
            mPluginQueue.erase(it);
            it = mPluginQueue.find(groupName);
        }
        return true;
    } else {
        return false;
    }
}

bool PluginManager::unregisterPluginLoader(const String &groupName) {
    PluginLoader *plugin = getPluginLoader(groupName);
    if ( plugin ) {
        // if pluginloader found delete the plugin and the map row
        delete plugin;
        mPluginLoaders.erase(groupName);
        return true;
    } else {
        // if pluginloader not found
        return false;
    }
}

void PluginManager::setPluginFolder(const String &folder) {
    mPluginFolder = folder + "/";
}

const String& PluginManager::getPluginFolder() {
    return mPluginFolder;
}

bool PluginManager::unregisterPluginLoader(PluginLoader *loader) {
    for ( PluginLoaders::iterator it = mPluginLoaders.begin();
          it != mPluginLoaders.end();
          ++it ) {
        if (it->second == loader) {
            // plugin found
            delete it->second;
            mPluginLoaders.erase(it);
            return true;
        }
    }
    return false;
}

PluginLoader* PluginManager::getPluginLoader(const String &groupName) {
    PluginLoaders::iterator pluginIt = mPluginLoaders.find(groupName);
    if ( pluginIt != mPluginLoaders.end()) {
        return pluginIt->second;
    } else {
        return NULL;
    }
}

bool PluginManager::loadPlugin(const String& group, const String& pluginPath) {
    PluginLoader* pl = getPluginLoader(group);
    if (pl) {
        // load the plugin
        pl->loadPlugin(pluginPath);
        return true;
    } else {
        // queue the plugin
        mPluginQueue.insert(std::pair<String, String>(
                            group.c_str(),
                            pluginPath.c_str()));
        LogManager::getSingletonPtr()->logMessage("Plugin Queued: "+pluginPath);
        return false;
    }
}

bool PluginManager::unloadPlugin(const String& group, const String& pluginPath) {
    PluginLoader* pl = getPluginLoader(group);
    if (pl) {
        // Plugin manager found: unload the plugin
        pl->unloadPlugin(pluginPath);
        return true;
    } else {
        // Search for the pluginPath in the plugin queue
        PluginQueue::iterator it = mPluginQueue.find(group);
        while (it != mPluginQueue.end()) {
            // if the plugin is found in the queue erase it
            if (it->second == pluginPath) {
                mPluginQueue.erase(it);
                return true;
            }
            it = mPluginQueue.find(group);
        }
        return false;
    }
}


bool PluginManager::isPluginLoaded(const String &pluginPath) {
    bool found = false;
    for (PluginLoaders::iterator it = mPluginLoaders.begin();
         (it != mPluginLoaders.end()) && (found == false);
         ++it) {
        PluginLoader* pl = it->second;
        if ( pl->isPluginLoaded(pluginPath) ) return true;
    }
    // end reached -> plugin not found
    return false;
}


void PluginManager::loadPluginsFromFile(const String &pluginsFileName) {
    LogManager::getSingletonPtr()->logMessage("*PLUGIN MANAGER: getting plugins to load");
    Ogre::ConfigFile cf;
    cf.load(pluginsFileName);

    // set the plugin folder used
    setPluginFolder( cf.getSetting("PluginFolder") );

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, pluginPath;
    // Iterate for every section, meaning for every diferent PluginLoader
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        // QUEUE PLUGINS
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            // check the type defined is plugin
            typeName = i->first;
            int equal;
            equal = stricmp(typeName.c_str(), "plugin");

            // If the type defined is plugin go ahead
            if ( equal == 0 )
            {
                // load the plugin or queue it untill the plugin loader is registered
                loadPlugin(secName, mPluginFolder + i->second);
            }
        }

    }
}
