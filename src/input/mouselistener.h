/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_MOUSELISTENER_H_
#define SRC_INPUT_MOUSELISTENER_H_

#include "EnginePrerequisites.h"
#include <OIS/OISMouse.h>

namespace Caelum {

/** Mouse Class
    Intermediate class between OIS Mouse Listener and Game Own listeners
    provide the same interface as MouseListener for compatibility, but offers an
    extra flexibility for other devices such as joysticks used as mouse.
  */

class MouseListener : public OIS::MouseListener {
  public:
    MouseListener();

    /** Start Listening
        Adds the currrent object to OIS listener list and starts receiving events
        @remarks use this only if you want to receive events all the time. Instead
        you can receive
      */
    void startListeningMouse();

    /// OIS MouseListeners
    virtual bool mouseMoved    (const OIS::MouseEvent& evt) { return true;}
    virtual bool mousePressed  (const OIS::MouseEvent& evt, OIS::MouseButtonID id) { return true;}
    virtual bool mouseReleased (const OIS::MouseEvent& evt, OIS::MouseButtonID id) { return true;}
    virtual bool mouseClick    (const OIS::MouseEvent& evt, OIS::MouseButtonID id) { return true;}

  private:
};
}  // namespace NAGE

#endif  // SRC_INPUT_MOUSELISTENER_H_
