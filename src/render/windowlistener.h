#ifndef SRC_RENDER_WINDOWLISTENER_H
#define SRC_RENDER_WINDOWLISTENER_H

#include "EnginePrerequisites.h"

#include "render/renderwindow.h"

namespace Caelum {

class WindowListener {
  public:
    virtual ~WindowListener();

    /**
    @remarks
        Window has moved position
    */
    virtual void windowMoved(RenderWindow* rw) { (void)rw;}
    /**
    @remarks
        Window has resized
    @param rw
        The RenderWindow which created this events
    */
    virtual void windowResized(RenderWindow* rw) { (void)rw;}
    /**
    @remarks
        Window is closing (Only triggered if user pressed the [X] button)
    @return True will close the window(default).
    */
    virtual bool windowClosing(RenderWindow* rw) { (void)rw; return true;}
    /**
    @remarks
        Window has been closed (Only triggered if user pressed the [X] button)
    @note
        The window has not actually close yet when this event triggers. It's only closed after
        all windowClosed events are triggered. This allows apps to deinitialise properly if they
        have services that needs the window to exist when deinitialising.
    */
    virtual void windowClosed(RenderWindow* rw) { (void)rw;}
    /**
    @remarks
        Window has lost/gained focus
    */
    virtual void windowFocusChange(RenderWindow* rw) { (void)rw;}
};

}

#endif // WINDOWLISTENER_H
