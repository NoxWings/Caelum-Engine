#ifndef AUDIO_AUDIOBUFFER_H_
#define AUDIO_AUDIOBUFFER_H_

#include "EnginePrerequisites.h"

namespace Caelum {

class AudioBuffer {
  public:
    /// @param name
    /// @param path where the song is located
    AudioBuffer(const String &fileName);
    ~AudioBuffer();

    const String& getFileName() { return mFileName;}
    uint* buffer() { return &mBuffer;}

  private:
    String mFileName;
    uint mBuffer;
};

} // namespace Caelum

#endif // AUDIO_AUDIOBUFFER_H_
