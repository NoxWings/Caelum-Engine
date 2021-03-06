/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_CORE_RESOURCEMANAGER_H_
#define SRC_CORE_RESOURCEMANAGER_H_

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "core/log.h"

namespace  Caelum {

class ResourceManager : public Singleton<ResourceManager> {
  public:
    static String DEFAULT_RESOURCE_GROUP_NAME;
    static String INTERNAL_RESOURCE_GROUP_NAME;
    static String AUTODETECT_RESOURCE_GROUP_NAME;

    ResourceManager();
    ResourceManager(const String &resourcesFileName);
    ~ResourceManager();

    /// ADDING RESOURCES

    void addResourceLocation(const String &archName,
                             const String &typeName,
                             const String &secName,
                             bool recursive = false);
    void setupResources(const String &resourcesFileName = "resources.cfg");

    ///

    bool resourceExists(const String &resourceGroupName, const String &fileName);

    /// INITIALISE RESOURCES
    void initialiseAllResources();
  private:
    Log* mLog;
};

}
#endif // SRC_CORE_RESOURCEMANAGER_H
