#include "audio/audiosource.h"

#include "audio/audiomanager.h"

#include <AL/al.h>
#include <string.h>

using namespace Caelum;

AudioSource::AudioSource(const String &name)
    : MovableComponent(name) {
    alGenSources(1, &mSource);
    setAudioBuffer(0);
    setVolume(1.0f);
}

AudioSource::AudioSource(const String &name, const String &bufferName)
    : MovableComponent(name) {
    alGenSources(1, &mSource);
    setAudioBuffer(bufferName);
    setVolume(1.0f);
}

AudioSource::AudioSource(const String &name, AudioBuffer *buffer)
    : MovableComponent(name) {
    alGenSources(1, &mSource);
    setAudioBuffer(buffer);
    setVolume(1.0f);
}

AudioSource::~AudioSource() {
    alSourceStop(mSource);
    alDeleteSources(1, &mSource);
}

void AudioSource::play() { alSourcePlay(mSource);}

bool AudioSource::isplaying() {
    ALenum state;
    alGetSourcei(mSource, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

void AudioSource::pause() { alSourcePause(mSource);}

void AudioSource::stop() { alSourceStop(mSource);}

void AudioSource::setVolume(float num) {
    alSourcef(mSource, AL_PITCH, 1.0f);
    alSourcef(mSource, AL_GAIN, num);
}

Real AudioSource::getVolume() {
    Real volume = 0.0f;
    alGetSourcef(mSource, AL_GAIN, &volume);
    return volume;
}

void AudioSource::setLoop(bool enable)  { alSourcei(mSource, AL_LOOPING, enable);}

bool AudioSource::getLoop() {
    int loop = 0;
    alGetSourcei(mSource, AL_LOOPING, &loop);
    return (loop != 0);
}

bool AudioSource::setAudioBuffer(const String &bufferName) {
    AudioBuffer *buffer = AudioManager::getSingleton().getBuffer(bufferName);
    setAudioBuffer(buffer);
    return (buffer);
}

void AudioSource::setAudioBuffer(AudioBuffer *audioBuffer) {
    mAudioBuffer = audioBuffer;
    if (mAudioBuffer) {
        alSourcei(mSource, AL_BUFFER, *(mAudioBuffer->buffer()));
    }
}

AudioBuffer *AudioSource::getAudioBuffer() {
    return mAudioBuffer;
}

void AudioSource::updatePosition(const Vector3 &position) {
    mVelocity[0] = 0; mVelocity[1] = 0; mVelocity[2] = 0;
    mPosition[0] = position.x;
    mPosition[1] = position.y;
    mPosition[2] = position.z;
    alSourcefv (mSource, AL_POSITION, mPosition);
    alSourcefv (mSource, AL_VELOCITY, mVelocity);
}

void AudioSource::updateOrientation(const Quaternion &orientation) {
    Vector3 lookAt = -orientation.zAxis();
    mOrientation[0] = lookAt.x;
    mOrientation[1] = lookAt.y;
    mOrientation[2] = lookAt.z;
    alSourcefv (mSource, AL_DIRECTION, mOrientation);
}

void AudioSource::updateScale(const Vector3 &scale) {
    // TODO add more gain ¿?
}
