/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_INPUTMANAGER_H_
#define SRC_INPUT_INPUTMANAGER_H_

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "input/keylistener.h"
#include "input/mouselistener.h"

namespace Caelum {

class InputManager : public Singleton<InputManager>,
        public KeyListener, public MouseListener {
  public:
    InputManager() {}
    virtual ~InputManager() {}

    // Add Listener
    virtual void addKeyListener         (KeyListener*   keyListener) = 0;
    virtual void addMouseListener       (MouseListener* mouseListener) = 0;
    virtual void addJoystickListener    () = 0;

    // Remove Listener
    virtual void removeKeyListener      (KeyListener*   keyListener) = 0;
    virtual void removeMouseListener    (MouseListener* mouseListener) = 0;
    virtual void removeJoystickListener () = 0;

    // MouseEvents
    virtual bool mouseMoved    (const MouseEvent& evt) = 0;
    virtual bool mousePressed  (const MouseEvent& evt, MouseButtonID id) = 0;
    virtual bool mouseReleased (const MouseEvent& evt, MouseButtonID id) = 0;
    virtual bool mouseClicked  (const MouseEvent& evt, MouseButtonID id) = 0;

    // KeyEvents
    virtual bool keyPressed  (const KeyEvent &evt) = 0;
    virtual bool keyReleased (const KeyEvent &evt) = 0;
    virtual bool keyTap      (const KeyEvent &evt) = 0;

    // Update()
    virtual void update() = 0;
};

}

#endif // SRC_INPUT_INPUTMANAGER_H_
