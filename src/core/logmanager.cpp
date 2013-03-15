/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "core/logmanager.h"

using namespace Caelum;

LogManager::LogManager()
    :  mainLog(0) {
}

LogManager::~LogManager() {
    logs.clear();
    mainLog = 0;
}

Log* LogManager::createDefaultLog(const String &name,
                                  bool debuggerOutput,
                                  bool fileOutput) {
    // Create the log
    Log* log = createConcreteLog(name, true,
                                 debuggerOutput,
                                 false,
                                 fileOutput);

    // Assign default log
    mainLog = log;
    // Add the log to the maps
    logs.insert(std::pair<const char*, Log*>(
                    log->getName().c_str(),
                    log));
    return log;
}

Log* LogManager::createLog(const String& name,
                           bool debuggerOutput,
                           bool pushToDefaultLog,
                           bool fileOutput) {
    // Create the log
    Log* log = createConcreteLog(name, false,
                                 debuggerOutput,
                                 pushToDefaultLog,
                                 fileOutput);
    // Add the log to the maps
    logs.insert(std::pair<const char*, Log*>(
                    log->getName().c_str(),
                    log));
    return log;
}

void LogManager::destroyLog(Log* log) {
    // Dereference mainLog
    if (log == mainLog) mainLog = 0;
    // Delete the map position
    logs.erase(log->getName().c_str());
    // Delete the log
    deleteConcreteLog( log );
}

void LogManager::destroyLog(const String& name) {
    destroyLog(getLog(name));
}

Log* LogManager::getDefaultLog() {
    return mainLog;
}

Log* LogManager::getLog(const String &name) {
    LogMap::iterator it = logs.find(name.c_str());
    Log* temp = NULL;
    if (it != logs.end()) {
        temp = it->second;
    }
    return temp;

}

/// LogMessage
void LogManager::logMessage(const String& message) {
    mainLog->logMessage(message);
}

