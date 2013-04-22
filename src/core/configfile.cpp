/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "configfile.h"

#include <OGRE/OgreConfigFile.h>

using namespace Caelum;

ConfigFile::ConfigFile() {

}

ConfigFile::~ConfigFile() {
    clear();
}

void ConfigFile::loadDirect(const String &filename,
                      const String &separators,
                      bool trimWhitespace) {
    // clearPrevious Settings
    clear();

    Ogre::ConfigFile cf;
    // direct load
    cf.load(filename, separators, trimWhitespace);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, keyName, value;
    // Iterate for every section
    while (seci.hasMoreElements())
    {
        // Get the name and multimap of the section
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

        // Insert section in our local var
        SettingsMultiMap* localSettings = new SettingsMultiMap;
        mSections.insert( std::pair<String, SettingsMultiMap*> (secName, localSettings));

        // Make a deep copy of settings ¿*localSettings = *settings; or iterate?
        for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin();
             i != settings->end();
             ++i)
        {
            // check the type defined is plugin
            keyName = i->first;
            value = i->second;
            // Insert settings in the local section multimap
            localSettings->insert(std::pair<String, String>(keyName, value));
        }
    }
}

void ConfigFile::load(const String &filename,
                      const String &resourceGroup,
                      const String &separators,
                      bool trimWhitespace) {
    // clearPrevious Settings
    clear();

    Ogre::ConfigFile cf;
    // direct load
    cf.load(filename, resourceGroup, separators, trimWhitespace);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, keyName, value;
    // Iterate for every section
    while (seci.hasMoreElements())
    {
        // Get the name and multimap of the section
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

        // Insert section in our local var
        SettingsMultiMap* localSettings = new SettingsMultiMap;
        mSections.insert( std::pair<String, SettingsMultiMap*> (secName, localSettings));

        // Make a deep copy of settings ¿*localSettings = *settings; or iterate?
        for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin();
             i != settings->end();
             ++i)
        {
            // check the type defined is plugin
            keyName = i->first;
            value = i->second;
            // Insert settings in the local section multimap
            localSettings->insert(std::pair<String, String>(keyName, value));
        }
    }
}

const String ConfigFile::getSetting(const String &key, const String &section) {
    SectionMap::iterator seci = mSections.find(section);
    if (seci != mSections.end()) {
        SettingsMultiMap* settings = seci->second;
        SettingsMultiMap::iterator setti = settings->find(key);
        if (setti != settings->end()) {
            return setti->second;
        }
    }
    return "";
}

const StringVector ConfigFile::getMultiSetting(const String &key, const String &section) {
    StringVector ret;

    SectionMap::const_iterator seci = mSections.find(section);
    if (seci != mSections.end())
    {
        SettingsMultiMap::const_iterator i;

        i = seci->second->find(key);
        // Iterate over matches
        while (i != seci->second->end() && i->first == key)
        {
            ret.push_back(i->second);
            ++i;
        }
    }
    return ret;
}

void ConfigFile::clear() {
    // clear every section settings
    for (SectionMap::iterator it = mSections.begin();
         it != mSections.end();
         ++it)
    {
        SettingsMultiMap* settings = it->second;
        settings->clear();
        delete settings;
    }
    // clear the sections
    mSections.clear();
}
