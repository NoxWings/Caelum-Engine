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

#include "core/logmanager.h"
#include "render/renderwindow.h"
#include "input/keylistener.h"
#include "input/mouselistener.h"

namespace Caelum {

class InputManager : public Singleton<InputManager>, public WindowListener {
  public:
    InputManager() { mLog = LogManager::getSingletonPtr()->createLog("input.log");}
    virtual ~InputManager() { LogManager::getSingletonPtr()->destroyLog(mLog);}

    // Add Listener
    virtual void addKeyListener         (Caelum::KeyListener*   keyListener) = 0;
    virtual void addMouseListener       (Caelum::MouseListener* mouseListener) = 0;
    virtual void addJoystickListener    () = 0;

    // Remove Listener
    virtual void removeKeyListener      (Caelum::KeyListener*   keyListener) = 0;
    virtual void removeMouseListener    (Caelum::MouseListener* mouseListener) = 0;
    virtual void removeJoystickListener () = 0;

    // Update()
    virtual void update() = 0;

    /// Window Handling
    /// We may need these events to recalculate the mouse clip space
    virtual void windowResized(RenderWindow* rw) = 0;
    virtual void windowClosed(RenderWindow *rw) = 0;

  protected:
    Log *mLog;
};

}

#endif // SRC_INPUT_INPUTMANAGER_H_
