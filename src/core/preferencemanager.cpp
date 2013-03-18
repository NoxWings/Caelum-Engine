#include "preferencemanager.h"

#include <stdio.h> // for the sscanf call

using namespace Caelum;

PreferenceManager::PreferenceManager() {
    clearPreferences();
}

PreferenceManager::PreferenceManager(const String &fileName) {
    loadPreferences(fileName);
}

PreferenceManager::PreferenceManager(const String &fileName, const String &resourceGroup) {
    loadPreferences(fileName, resourceGroup);
}

PreferenceManager::~PreferenceManager() {
    clearPreferences();
}

void PreferenceManager::loadPreferences(const String &fileName) {
    // load the file
    cf.loadDirect(fileName);
    // set the preferences
    loadOptions();
}

void PreferenceManager::loadPreferences(const String &fileName, const String &resourceGroup) {
    // load the file
    cf.load(fileName, resourceGroup);
    // set the preferences
    loadOptions();
}


void PreferenceManager::loadOptions() {
    // clear every variable
    clearPreferences();
    loadRenderOptions();
}

void PreferenceManager::loadRenderOptions() {
    mWindowName = cf.getSetting("Window Name","Render"); // render system under Render
    mRenderSystem = cf.getSetting("Render System","Render"); // render system under Render

    // set the global video options for the selected Render System
    String name, val;
    for (ConfigFile::SettingsMultiMap::iterator it = cf.getSettingsItBegin(mRenderSystem);
         it != cf.getSettingsItEnd(mRenderSystem);
         ++it) {
        name = it->first;
        val = it->second;
        mVideoOptions.insert(VideoOptions::value_type(name,val));
    }

    // Get the resolution and fullscreen
    val = cf.getSetting("Resolution", mRenderSystem);
    sscanf(val.c_str(), "%dx%d", &mResolutionW, &mResoulutionH);
    mVideoOptions.erase("Resolution");

    val = cf.getSetting("FullScreen", mRenderSystem);
    mFullScreen = StringUtils::equals(val, "Yes")?true:false;
    mVideoOptions.erase("FullScreen");
}

void PreferenceManager::clearPreferences() {
    clearRenderOptions();
}


void PreferenceManager::clearRenderOptions() {
    mRenderSystem = "";
    mResolutionW = 0;
    mResoulutionH = 0;
    mFullScreen = false;
    mVideoOptions.clear();
    mWindowName = "";
}
