#ifndef SRC_RENDER_OGRE_OGRERENDERWINDOW_H
#define SRC_RENDER_OGRE_OGRERENDERWINDOW_H

#include "EnginePrerequisites.h"

#include "render/renderwindow.h"
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreWindowEventUtilities.h>

namespace Caelum {

class OgreRenderWindow : public RenderWindow, public Ogre::WindowEventListener {
  public:
    OgreRenderWindow(const String& windowName,
                     unsigned int resolutionW,
                     unsigned int resolutionH,
                     bool fullscreen,
                     VideoOptions* videoOpts);
    virtual ~OgreRenderWindow();

    virtual void addViewPort();

    // ************************************
    // Ogre Window Event Listener Interface
    // ************************************
    /**
    @remarks
        Window has moved position
    */
    virtual void windowMoved(Ogre::RenderWindow* rw);
    /**
    @remarks
        Window has resized
    @param rw
        The RenderWindow which created this events
    */
    virtual void windowResized(Ogre::RenderWindow* rw);
    /**
    @remarks
        Window is closing (Only triggered if user pressed the [X] button)
    @return True will close the window(default).
    */
    virtual bool windowClosing(Ogre::RenderWindow* rw);
    /**
    @remarks
        Window has been closed (Only triggered if user pressed the [X] button)
    @note
        The window has not actually close yet when this event triggers. It's only closed after
        all windowClosed events are triggered. This allows apps to deinitialise properly if they
        have services that needs the window to exist when deinitialising.
    */
    virtual void windowClosed(Ogre::RenderWindow* rw);
    /**
    @remarks
        Window has lost/gained focus
    */
    virtual void windowFocusChange(Ogre::RenderWindow* rw);

  private:
    Ogre::RenderWindow* mRenderWindow;
};

}

#endif // SRC_RENDER_OGRE_OGRERENDERWINDOW_H
