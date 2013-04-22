#ifndef OGRERENDERMANAGER_H
#define OGRERENDERMANAGER_H

#include "EnginePrerequisites.h"

#include "render/rendermanager.h"
#include <OGRE/Ogre.h>

namespace Caelum {

class OgreRenderManager : public RenderManager, public Ogre::FrameListener {
  public:
    OgreRenderManager();
    ~OgreRenderManager();

    void startRendering();

    bool frameStarted(const Ogre::FrameEvent& evt);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool frameEnded(const Ogre::FrameEvent& evt);

  private:
    Ogre::Root *mRoot;
};

}

#endif // OGRERENDERMANAGER_H
