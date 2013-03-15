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


namespace Caelum {

class PluginLoader {
  public:
    virtual ~PluginLoader() {}

    virtual void loadPlugin(const String& pluginPath) = 0;
    virtual void unloadPlugin(const String& pluginPath) = 0;
    virtual bool isPluginLoaded(const String& pluginPath) = 0;
};

}

#endif // SRC_CORE_PLUGINLOADER_H_
