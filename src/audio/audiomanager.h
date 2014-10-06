#ifndef AUDIO_AUDIOMANAGER_H_
#define AUDIO_AUDIOMANAGER_H_

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "game/layermanager.h"

#include "audio/audiolayer.h"
#include "audio/audiobuffer.h"

struct ALCdevice_struct;
typedef struct ALCdevice_struct ALCdevice;

namespace Caelum {

class GameEngine;

class AudioManager :
        public Singleton<AudioManager>,
        public LayerManager {
  public:
    AudioManager(const String &defaultPath = "../media/sound/");
    virtual ~AudioManager();

    GameLayer* createLayer(const String &name, const String& typeName);
    AudioLayer* createAudioLayer(const String &name, const String &typeName);

    /// Set the resource folder where load files
    void setResourceFolder(const String &path);

    /// Look if sound its loaded & return its buffer,
    /// in case the sound its not loaded this funcion will load it
    /// @return Loaded Buffer
    AudioBuffer* getBuffer(const String &fileName);

    ALCdevice* getCurrentDevice() {return mDevice;}

  private:
    /// Load and especific sound & return its buffer
    /// @return Loaded Buffer
    AudioBuffer* loadBuffer(const String &fileName);
    void unloadAllBuffers();

    Log* mLog;
    GameEngine* mEngine;
    ALCdevice *mDevice;
    // List of Buffer sounds
    std::map<String, AudioBuffer*> mBufferMap;
    std::string mDefaultLocation;
};

} // end of namespace Caelum
#endif // AUDIO_RESOURCES_HPP
