/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_CORE_LOGMANAGER_H
#define SRC_CORE_LOGMANAGER_H

#include "EnginePrerequisites.h"
#include "patterns/Singleton.h"
#include "core/log.h"

#include <map>

namespace Caelum {

class LogManager : public Singleton<LogManager> {
  public:
    /// Constructor
    LogManager();
    /// Destructor
    virtual ~LogManager();

    /// Create Log
    Log* createDefaultLog(const String& name,
                          bool debuggerOutput = true,
                          bool fileOutput = true);

    Log* createLog(const String& name,
                   bool debuggerOutput = false,
                   bool pushToDefaultLog = true,
                   bool fileOutput = true);

    /// Destroy Log
    void destroyLog(Log* log);
    void destroyLog(const String& name);

    /// GetLog
    Log* getDefaultLog();
    Log* getLog(const String& name);

    /// LogMessage
    void logMessage(const String& message);

  protected:
    virtual Log* createConcreteLog(const String& name,
                           bool defaultLog = false,
                           bool debuggerOutput = true,
                           bool pushToDefaultLog = true,
                           bool fileOutput = true) = 0;

    virtual void deleteConcreteLog(Log* log) = 0;

    typedef std::map<const char*, Log*> LogMap;
    LogMap logs;
    Log* mainLog;
};

}
#endif // SRC_CORE_LOGMANAGER_H
