/*
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  This file is part of NAGE.
 */

#ifndef INPUT_KEYBOARD_HPP
#define INPUT_KEYBOARD_HPP

#include <OIS/OISKeyboard.h>

namespace NAGE {

class KeyListener : public OIS::KeyListener {
public:
    KeyListener();

    /** Start Listening
        Adds the currrent object to OIS listener list and starts receiving events
        @remarks use this only if you want to receive events all the time. Instead
        you can receive
      */
    void startListeningKey();

    /// OIS KeyListeners
    virtual bool keyPressed  (const OIS::KeyEvent &arg) {return true;}
    virtual bool keyReleased (const OIS::KeyEvent &arg) {return true;}
    virtual bool keyTap      (const OIS::KeyEvent &arg) {return true;}
};

}
#endif // INPUT_KEYBOARD_HPP
