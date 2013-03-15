/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "core/ogre/ogrelog.h"

using namespace Caelum;

OgreLog::OgreLog(Ogre::LogManager* logMan,
                 const String& name,
                 bool defaultLog = false,
                 bool debuggerOutput = true,
                 bool pushToDefaultLog = true,
                 bool suppressFileOutput = false)
    : mLogMan(logMan) {
    mPushToDefault = pushToDefaultLog;
    mLog = mLogMan->createLog(name, defaultLog, debuggerOutput, suppressFileOutput);
}

OgreLog::~OgreLog() {
    // destroy log
    mLogMan->destroyLog(mLog);
    // derreference log and logmanager
    mLog = 0;
    mLogMan = 0;
}

const String& OgreLog::getName() {
    return mLog->getName();
}


void OgreLog::logMessage(const String &message) {
    mLog->logMessage(message);
    // if I am not the default log, send a message to the default log too
    if (mPushToDefault) mLogMan->logMessage(message);
}
