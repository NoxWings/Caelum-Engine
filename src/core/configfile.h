/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "EnginePrerequisites.h"
#include "util/stringutils.h"

#include <map>

namespace Caelum {


/** ConfigFile
 * @brief The ConfigFile class
 */
class ConfigFile {
  public:
    ConfigFile();
    ~ConfigFile();

    /**
     * @brief load : Loads the indicated configuration file directly from the path selected in filename
     * @param filename the file path to look for the configuration file
     * @param separators allowed separator for name-value pairs
     * @param trimWhitespace if true trims the previous and last white spaces of the name and values
     */
    void loadDirect(const String& filename, const String& separators = "\t:=", bool trimWhitespace = true);
    /**
     * @brief load : Loads and parses the indicated configuration file previously located in the resource system
     * @param filename the file path to look for the configuration file
     * @param resourceGroup the resourcegroup where the config file is located
     * @param separators allowed separator for name-value pairs
     * @param trimWhitespace if true trims the previous and last white spaces of the name and values
     */
    void load(const String& filename, const String& resourceGroup, const String& separators = "\t:=", bool trimWhitespace = true);

    /**
     * @brief getSetting : get a single value from a setting with name "key"
     * @param key the name to look for in the config file
     * @param section the section to look for the key name [this is optional]
     * @return the value for the specified key if any
     */
    const String       getSetting(const String& key, const String& section = "") ;
    /**
     * @brief getMultiSetting : get every value from a setting with name "key"
     * @param key the name to look for in the config file
     * @param section the section to look for the key name [this is optional]
     * @return a vector of the values found for the specified key if any
     */
    const StringVector getMultiSetting(const String& key, const String& section = "");

    /// ITERATORS
    typedef std::multimap<String, String> SettingsMultiMap;
    typedef std::map<String, SettingsMultiMap*> SectionMap;

    const SectionMap::iterator getSectionItBegin() { return mSections.begin();}
    const SectionMap::iterator getSectionItEnd() { return mSections.end();}
    const SettingsMultiMap::iterator getSettingsItBegin(const String& section) {
        SectionMap::iterator seci = mSections.find(section);
        if (seci != mSections.end()) return seci->second->begin();
        // workaround as we cannot return NULL if the section has not been found
        // the begin and end settings iterator will be the same to prevent any looping
        return mSections.end()->second->end();
    }
    const SettingsMultiMap::iterator getSettingsItEnd(const String& section) {
        SectionMap::iterator seci = mSections.find(section);
        if (seci != mSections.end()) return seci->second->end();
        // workaround as we cannot return NULL if the section has not been found
        // the begin and end settings iterator will be the same to prevent any looping
        return mSections.end()->second->end();
    }


  protected:
    void clear();
    SectionMap mSections;
};

}

#endif // CONFIGFILE_H
