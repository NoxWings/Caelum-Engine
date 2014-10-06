#ifndef SRC_AUDIO_AUDIORECEPTOR_H_
#define SRC_AUDIO_AUDIORECEPTOR_H_

#include "EnginePrerequisites.h"

#include "game/movablecomponent.h"

namespace Caelum {

class AudioReceptor :
        public MovableComponent {
  public:
    AudioReceptor();
    virtual ~AudioReceptor();

    void updatePosition(const Vector3& position);
    void updateOrientation(const Quaternion& orientation);
    void updateScale(const Vector3& scale);

  protected:
    Real mPosition[3];
    Real mVelocity[3];
    Real mOrientation[6];
};

} // end of namespace Caelum

#endif // SRC_AUDIO_AUDIORECEPTOR_H_
