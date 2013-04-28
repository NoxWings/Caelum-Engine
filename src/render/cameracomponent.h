#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "EnginePrerequisites.h"

#include "render/rendercomponent.h"

namespace Ogre {
class Camera;
}

namespace Caelum {

class CameraComponent : public RenderComponent {
  public:
    CameraComponent(const String& name, RenderLayer* renderlayer);
    virtual ~CameraComponent();

    const Radian& getFOVy();
    void setFOVy(const Radian &fov);

    void setPerspectiveProjection();
    void setOrtographicProjection();

    void setNearClipDistance(Real near);
    void setFarClipDistance(Real far);

    void setAsActiveCamera();

    friend class OgreRenderWindow;
  private:
    Ogre::MovableObject* _getMovableObject();
    Ogre::Camera* _getCamera();

    Ogre::Camera *mCam;
};

}

#endif // CAMERACOMPONENT_H
