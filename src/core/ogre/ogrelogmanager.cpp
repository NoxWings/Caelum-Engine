/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "core/ogre/ogrelogmanager.h"
#include "core/ogre/ogrelog.h"

using namespace Caelum;

OgreLogManager::OgreLogManager() {
    mLogMan = new Ogre::LogManager();
}

OgreLogManager::~OgreLogManager() {
    // Delete every log
    deleteAndClearLogs();
    // We have to delete the pointer of the singleton because ogre may have deleted the log already
    // and therefore mLogMan could be pointing to an undefined position resulting into a crash
    delete Ogre::LogManager::getSingletonPtr();
    mLogMan = 0;
}

Log* OgreLogManager::createConcreteLog(const String& name,
                                       bool defaultLog,
                                       bool debuggerOutput,
                                       bool pushToDefaultLog,
                                       bool fileOutput) {
    // Create abstraction logger
    return (new OgreLog(mLogMan,
                        name,
                        defaultLog,
                        debuggerOutput,
                        pushToDefaultLog,
                        !fileOutput));
}

void OgreLogManager::deleteConcreteLog(Log *log) {
    delete log;
}

void OgreLogManager::deleteAndClearLogs() {
    //  **NOTE: Very important to delete every log here, since we cannot delete
    // the inner ogre logs without the ogre log manager
    for ( LogMap::iterator it = logs.begin(); it != logs.end(); ++it ) {
        delete it->second;
    }
    logs.clear();
}
