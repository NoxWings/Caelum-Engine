#ifndef PREFERENCEMANAGER_H
#define PREFERENCEMANAGER_H

#include "EnginePrerequisites.h"
#include "patterns/Singleton.h"
#include "core/configfile.h"
#include "render/renderwindow.h"


namespace Caelum {

class PreferenceManager : public Singleton<PreferenceManager> {
  public:
    PreferenceManager();
    PreferenceManager(const String& fileName);
    PreferenceManager(const String& fileName, const String& resourceGroup);
    ~PreferenceManager();

    void loadPreferences(const String& fileName);
    void loadPreferences(const String& fileName, const String& resourceGroup);

    /// Preference getters
    const String getSetting(const String& key, const String& section) { return cf.getSetting(key, section);}

    const String& getWindowName()   { return mWindowName;}
    const String& getRenderSystem() { return mRenderSystem;}
    const unsigned int getResolutionWidth()  { return mResolutionW;}
    const unsigned int getResolutionHeight() { return mResoulutionH;}
    const bool getFullScreen()      { return mFullScreen;}
    VideoOptions* getVideoOptions() { return &mVideoOptions;}


  private:
    void loadOptions();
    void loadRenderOptions();

    void clearPreferences();
    void clearRenderOptions();

    ConfigFile cf;
    String mRenderSystem;
    unsigned int mResolutionW, mResoulutionH;
    bool mFullScreen;
    VideoOptions mVideoOptions;
    String mWindowName;

};

}

#endif // PREFERENCEMANAGER_H
