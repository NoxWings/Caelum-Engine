/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef OGRELOG_H
#define OGRELOG_H

#include "core/log.h"

#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreLog.h>

namespace Caelum {

class OgreLog : public Log {
  public:
    OgreLog(Ogre::LogManager* logMan,
            const String &name,
            bool defaultLog,
            bool debuggerOutput,
            bool pushToDefaultLog,
            bool suppressFileOutput);
    ~OgreLog();

    const String& getName();
    void logMessage(const String &message);

  private:
    bool mPushToDefault;
    Ogre::Log *mLog;
    Ogre::LogManager *mLogMan;
};

}

#endif // OGRELOG_H
