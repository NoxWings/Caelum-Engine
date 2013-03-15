/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef OGRELOGMANAGER_H
#define OGRELOGMANAGER_H

#include "core/logmanager.h"
#include <OGRE/OgreLogManager.h>

namespace Caelum {

class OgreLogManager : public LogManager {
  public:
    OgreLogManager();
    ~OgreLogManager();

  protected:

    virtual Log* createConcreteLog(const String& name,
                           bool defaultLog = false,
                           bool debuggerOutput = true,
                           bool pushToDefaultLog = true,
                           bool fileOutput = true);

    void deleteConcreteLog(Log* log);

  private:

    void deleteAndClearLogs();

    Ogre::LogManager* mLogMan;
};

}
#endif // OGRELOGMANAGER_H
