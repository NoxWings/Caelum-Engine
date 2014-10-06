#include "audio/audiomanager.h"

#include "core/gameengine.h"
#include "audio/audiodecoder.h"

#include <AL/al.h>
#include <AL/alc.h>

using namespace Caelum;

AudioManager::AudioManager(const String &defaultPath) {
    mEngine = GameEngine::getSingletonPtr();
    mLog = mEngine->getLogManager()->createLog("audio.log");
    mLog->logMessage("Initialising Audio Manager");
    mDefaultLocation = defaultPath;
    // INITIALIZE OPENAL
    // Open Default Device
    mDevice = alcOpenDevice(NULL);
    if (!mDevice) {
        mLog->logMessage("ERROR: error initializing");
    }

}

AudioManager::~AudioManager() {
    destroyAllLayers();
    unloadAllBuffers();
    if (mDevice) alcCloseDevice(mDevice);
    mEngine->getLogManager()->destroyLog(mLog);
    mLog = 0;
    mEngine = 0;
}

GameLayer* AudioManager::createLayer(const String &name, const String &typeName) {
    return createAudioLayer(name, typeName);
}

AudioLayer* AudioManager::createAudioLayer(const String &name, const String &typeName) {
    AudioLayer *audioLayer = new AudioLayer(name, typeName);
    addLayer(audioLayer);
    return audioLayer;
}

void AudioManager::setResourceFolder(const String &path) {
    mDefaultLocation = path;
}

AudioBuffer* AudioManager::getBuffer(const String &fileName) {
    std::map<String, AudioBuffer*>::iterator it = mBufferMap.find(fileName);
    if (it != mBufferMap.end()) {
        mLog->logMessage("BUFFER FOUND!");
        return it->second; // found
    } else {
        mLog->logMessage("BUFFER NOT FOUND.");
        return loadBuffer(fileName); // not found
    }
}

AudioBuffer* AudioManager::loadBuffer(const String &fileName) {
    mLog->logMessage("LOADING BUFFER: "+fileName);
    String path = mDefaultLocation + fileName;
    AudioBuffer *buffer = new AudioBuffer(fileName);
    mLog->logMessage("INITIAL BUFFER LOADED");
    AudioDecoder::loadOgg(path, buffer);
    mLog->logMessage("SOUND DECODED");
    mBufferMap[fileName] = buffer;
    mLog->logMessage("BUFFER STORED");
    return buffer;
}

void AudioManager::unloadAllBuffers() {
    // Delete buffers
    std::map<String, AudioBuffer*>::iterator it;
    for( it = mBufferMap.begin(); it != mBufferMap.end(); ++it ) {
        delete it->second;
    }
    // Clear List
    mBufferMap.clear();
}


