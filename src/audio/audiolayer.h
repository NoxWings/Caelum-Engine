#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include "game/gamelayer.h"
#include "audio/audioreceptor.h"
#include "audio/audiosource.h"
#include "core/log.h"

struct ALCdevice_struct;
typedef struct ALCdevice_struct ALCdevice;
struct ALCcontext_struct;
typedef struct ALCcontext_struct ALCcontext;

namespace Caelum {

class AudioManager;

class AudioLayer
        : public GameLayer {
public:
    AudioLayer(const String &name, const String &typeName);
    virtual ~AudioLayer();

    // Movable Components
    AudioSource* createAudioSource(const String &sourceName, const String &bufferFileName);
    AudioReceptor* createAudioReceptor();
    AudioReceptor* getAudioReceptor();

    void update(Real deltaTime);

    // Properties
    void setVolume(Real vol);
    Real getVolume();

    void setAsCurrent();

protected:
    bool init();
    void shutdown();

    Log *mLog;
    AudioManager *mAudioMan;
    AudioReceptor *mReceptor;
    ALCdevice *mDevice;
    ALCcontext *mContext;
    Real mVolume;
};

} // namespace Caelum

#endif // AUDIO_MANAGER_H
