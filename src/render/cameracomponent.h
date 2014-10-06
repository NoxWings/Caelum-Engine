#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "EnginePrerequisites.h"

#include "render/rendercomponent.h"

namespace Ogre {
class Camera;
}

namespace Caelum {

enum POLYGON_MODE {
    PM_POINTS = 1, /// Only points are rendered.
    PM_WIREFRAME = 2, /// Wireframe models are rendered.
    PM_SOLID = 3 /// Solid polygons are rendered.
};

class Camera : public RenderComponent {
  public:
    Camera(const String& name, RenderLayer* renderlayer);
    virtual ~Camera();

    const Radian& getFOVy();
    void setFOVy(const Radian &fov);

    void setPerspectiveProjection();
    void setOrtographicProjection();

    void setNearClipDistance(Real near);
    void setFarClipDistance(Real far);

    void setPolygonMode(POLYGON_MODE mode);
    POLYGON_MODE getPolygonMode();

    void setAsActiveCamera();

    Ogre::Camera* _getCamera();

    friend class OgreRenderWindow;
  private:
    Ogre::MovableObject* _getMovableObject();

    Ogre::Camera *mCam;
};

}

#endif // CAMERACOMPONENT_H
