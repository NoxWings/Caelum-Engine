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

namespace Caelum {

//! Button ID for mouse devices
enum MouseButtonID {
    MB_Left = 0, MB_Right, MB_Middle,
    MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7,
    NUM_MOUSE_BUTTONS // THIS ONE IS EXTRA ADDED FOR TIMER ALLOC COUNT
};

/** Mouse Events */
class MouseEvent {
  public:
    MouseEvent() {}
    virtual ~MouseEvent() {}
};

/** MouseListener Class
    Interface for mouse event reception. In order to recieve mouse events
    you must implement this interface and register it into the input manager.
  */
class MouseListener {
  public:
    MouseListener() {}
    virtual ~MouseListener() {}

    /// OIS MouseListeners
    virtual bool mouseMoved    (const MouseEvent& evt) = 0;
    virtual bool mousePressed  (const MouseEvent& evt, MouseButtonID id) = 0;
    virtual bool mouseReleased (const MouseEvent& evt, MouseButtonID id) = 0;
    virtual bool mouseClicked  (const MouseEvent& evt, MouseButtonID id) = 0;
};

}  // namespace Caelum

#endif  // SRC_INPUT_MOUSELISTENER_H_
