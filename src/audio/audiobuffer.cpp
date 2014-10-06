#include "audio/audiobuffer.h"

#include <AL/al.h>

using namespace Caelum;

AudioBuffer::AudioBuffer(const String &fileName) {
    mFileName =  fileName;
    // Generate Buffer
    alGenBuffers(1, &mBuffer);
}

AudioBuffer::~AudioBuffer() {
    alDeleteBuffers(1, &mBuffer);
}
