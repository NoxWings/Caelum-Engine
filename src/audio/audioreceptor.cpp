#include "audio/audioreceptor.h"

#include <AL/al.h>
#include <AL/alc.h>

using namespace Caelum;

AudioReceptor::AudioReceptor()
    : MovableComponent("") {
}

AudioReceptor::~AudioReceptor() {
}

void AudioReceptor::updatePosition(const Vector3 &position) {
    mVelocity[0] = 0; mVelocity[1] = 0; mVelocity[2] = 0;
    mPosition[0] = position.x;
    mPosition[1] = position.y;
    mPosition[2] = position.z;
    alListenerfv(AL_POSITION, mPosition);
    alListenerfv(AL_VELOCITY, mVelocity);
}

void AudioReceptor::updateOrientation(const Quaternion &orientation) {
    // TODO
    Vector3 lookat = -orientation.zAxis();
    Vector3 up = orientation.yAxis();
    mOrientation[0] = lookat.x;
    mOrientation[1] = lookat.y;
    mOrientation[2] = lookat.z;
    mOrientation[3] = up.x;
    mOrientation[4] = up.y;
    mOrientation[5] = up.z;
    alListenerfv(AL_ORIENTATION, mOrientation);
}

void AudioReceptor::updateScale(const Vector3 &scale) {
    // TODO add more gain ¿?
}
