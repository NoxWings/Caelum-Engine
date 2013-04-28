/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "resourcemanager.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreResourceGroupManager.h>

#include "core/logmanager.h"

using namespace Caelum;

String ResourceManager::DEFAULT_RESOURCE_GROUP_NAME = "General";
String ResourceManager::INTERNAL_RESOURCE_GROUP_NAME = "Internal";
String ResourceManager::AUTODETECT_RESOURCE_GROUP_NAME = "Autodetect";

ResourceManager::ResourceManager() {
    mLog = LogManager::getSingletonPtr()->getDefaultLog();
}

ResourceManager::ResourceManager(const String &resourcesFileName) {
    mLog = LogManager::getSingletonPtr()->getDefaultLog();
    setupResources(resourcesFileName);
}

ResourceManager::~ResourceManager() {
}

void ResourceManager::addResourceLocation(const String &archName,
                                          const String &typeName,
                                          const String &secName,
                                          bool recursive) {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName,
                                                                   typeName,
                                                                   secName,
                                                                   recursive);
}

void ResourceManager::setupResources(const String &resourcesFileName) {
    mLog->logMessage("*RESOURCE MANAGER: adding resource locations");
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(resourcesFileName);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            if (typeName.compare("RecFS") == 0) {
                typeName = "FileSystem";
                this->addResourceLocation(archName, typeName, secName, true);
            } else {
                this->addResourceLocation(archName, typeName, secName, false);
            }
        }
    }
}

void ResourceManager::initialiseAllResources() {
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
