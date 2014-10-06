#include "audio/audiolayer.h"

#include "audio/audiomanager.h"
#include "core/logmanager.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <stdlib.h>

using namespace Caelum;

AudioLayer::AudioLayer(const String &name, const String &typeName)
    : GameLayer(name, typeName), mReceptor(0) {
    mLog = LogManager::getSingleton().getLog("audio.log");
    mAudioMan = AudioManager::getSingletonPtr();
    // Initialize OpenAL
    if (!init()) {
        exit(-1);
    }
}

AudioLayer::~AudioLayer() {
    shutdown();
    mAudioMan = 0;
    mLog = 0;
}

AudioSource* AudioLayer::createAudioSource(const String &sourceName, const String &bufferFileName) {
    mLog->logMessage("CREATING AUDIO SOURCE");
    AudioBuffer *buffer = mAudioMan->getBuffer(bufferFileName);
    if (buffer != 0)
        mLog->logMessage("BUFFER RETRIEVED");
    AudioSource *source = new AudioSource(sourceName, buffer);
    mLog->logMessage("SOURCE CREATED");
    source->setVolume(mVolume);
    mLog->logMessage("VOLUME FIXED");
    this->addComponent(source);
    mLog->logMessage("COMPONENT STORED");
    return source;
}

AudioReceptor *AudioLayer::createAudioReceptor() {
    if (!mReceptor) {
        mReceptor = new AudioReceptor();
    }
    return mReceptor;
}

AudioReceptor *AudioLayer::getAudioReceptor() {
    return createAudioReceptor();
}

void AudioLayer::update(Real deltaTime) {
    // TODO
}

void AudioLayer::setVolume(Real vol) {
    mVolume = vol;
    AssociativeCollection<String, Component*>::ItemMap::iterator it;
    for ( it = mComponents.mItems.begin();
          it != mComponents.mItems.end();
          ++it) {
        AudioSource *c = (AudioSource*)(it->second);
        c->setVolume(mVolume);
    }
}

Real AudioLayer::getVolume() {
    return mVolume;
}

bool AudioLayer::init() {
    mDevice = mAudioMan->getCurrentDevice();

    // Create Context
    mContext = alcCreateContext (mDevice, NULL);
    if (mContext == NULL) {
        alcCloseDevice (mDevice);
        return false;
    }
    if (!alcMakeContextCurrent (mContext)) {
        alcDestroyContext (mContext);
        alcCloseDevice (mDevice);
        return false;
    }
    // Create the main listener
    createAudioReceptor();

    // Check if any error
    if (alGetError() != AL_NO_ERROR) {
        return false;
    }
    return true;
}

void AudioLayer::shutdown() {
    // delete components
    this->destroyAllComponents();

    // delete receptor
    delete mReceptor;

    if (mContext) {
        if (alcGetCurrentContext() == mContext)
            alcMakeContextCurrent(NULL); // Detach Context
        alcDestroyContext(mContext);
    }
    mDevice = 0;
}

void AudioLayer::setAsCurrent() {
    alcMakeContextCurrent(mContext);
}
