#ifndef AUDIO_AUDIOSOURCE_H_
#define AUDIO_AUDIOSOURCE_H_

#include "EnginePrerequisites.h"

#include "game/movablecomponent.h"
#include "audio/audiobuffer.h"

namespace Caelum {

class AudioSource
        : public MovableComponent {
public:
    AudioSource(const String &name);
    AudioSource(const String &name, const String &bufferName);
    AudioSource(const String &name, AudioBuffer *buffer);
    virtual ~AudioSource();

    void play();
    bool isplaying();
    void pause();
    void stop();
    void setVolume(float num);
    Real getVolume();
    void setLoop(bool enable);
    bool getLoop();
    bool setAudioBuffer(const String &bufferName);
    void setAudioBuffer(AudioBuffer *audioBuffer);
    AudioBuffer* getAudioBuffer();

    void updatePosition(const Vector3 &position);
    void updateOrientation(const Quaternion &orientation);
    void updateScale(const Vector3 &scale);

private:
    AudioBuffer *mAudioBuffer;
    uint  mSource;
    Real mPosition[3];
    Real mVelocity[3];
    Real mOrientation[3];
};

}
#endif // AUDIO_AUDIOSOURCE_H_
