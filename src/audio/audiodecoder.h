#ifndef AUDIO_LOADER_H_
#define AUDIO_LOADER_H_

#include "EnginePrerequisites.h"

#include "audio/audiobuffer.h"

namespace Caelum {

class AudioDecoder {
  public:
    /** Loads a wav sound file from the path.
        @return the buffer with the wav sound loaded */
    //static void loadWAV(const char* path, ALuint &buffer);
    /** Loads a ogg sound file from the path.
        @return the buffer with the ogg sound loaded */
    static void old_loadOgg(const String &path, AudioBuffer *buffer);
    static void loadOgg(const String &path, AudioBuffer *buffer);
  private:
    AudioDecoder() {}
};

} // namespace audio

#endif // AUDIO_LOADER_HPP
