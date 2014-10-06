/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_OISINPUTMANAGER_H_
#define SRC_INPUT_OISINPUTMANAGER_H_

// OIS
// Use this define to signify OIS will be used as a DLL
// (so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

// Lib
#include "EnginePrerequisites.h"

// InputManager
#include "input/inputmanager.h"

// RenderManager
#include "render/rendermanager.h"

// C++
#include <vector>
#include <list>
#include <map>

#define DEFAULT_CLICK_INTERVAL       250
#define DEFAULT_DOUBLECLICK_INTERVAL 300
#define DEFAULT_TAP_INTERVAL         250  // Time in miliseconds
#define DEFAULT_DOUBLETAP_INTERVAL   300  // Time in miliseconds


namespace Caelum {

/**
  */
class OISInputManager : public InputManager,// Inheritance
        // Listener Interfaces
        public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener {
  public:
    typedef std::vector<OIS::JoyStick*> JoyStickVector;
    typedef std::map<int, Caelum::JoyStickEvent*> JoyStickEventMap;

    OISInputManager();
    virtual ~OISInputManager();

    // Add Listener
    void addKeyListener         (Caelum::KeyListener *keyListener)     {mKeyListeners.push_back(keyListener);}
    void addMouseListener       (Caelum::MouseListener *mouseListener) {mMouseListeners.push_back(mouseListener);}
    void addJoystickListener    (Caelum::JoyStickListener* joyStickListener) {mJoyListeners.push_back(joyStickListener);}
    // Remove Listener
    void removeKeyListener      (Caelum::KeyListener *keyListener)     {mKeyListeners.remove(keyListener);}
    void removeMouseListener    (Caelum::MouseListener *mouseListener) {mMouseListeners.remove(mouseListener);}
    void removeJoystickListener (Caelum::JoyStickListener* joyStickListener) {mJoyListeners.remove(joyStickListener);}

    /// Update
    virtual void update();

    /// Window Modification Handle
    void windowResized(RenderWindow* rw);
    void windowClosed(RenderWindow *rw);

  protected:
    /// SETUP & SHUTDOWN
    void setup();
    void shutdown();
    // fallback emergency method
    static inline void _emergency_shutdown();

    /// Input system
    OIS::InputManager* createInputSystem(bool grabInput);
    OIS::Mouse*           createMouse    (bool buffered);
    OIS::Keyboard*        createKeyboard (bool buffered);
    const JoyStickVector& createJoySticks(bool buffered);
    void destroyInputSystem();
    void destroyMouse();
    void destroyKeyBoard();
    void destroyJoySticks();

    /// Mouse Listeners
    virtual bool mouseMoved   (const OIS::MouseEvent &arg);  // basic
    virtual bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // basic
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // basic
    virtual bool mouseClick   (const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // extended
    //virtual bool mouseDoubleClick (const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // extended

    /// Keyboard Listeners
    virtual bool keyPressed (const OIS::KeyEvent &arg);  // basic
    virtual bool keyReleased(const OIS::KeyEvent &arg);  // basic
    virtual bool keyTap     (const OIS::KeyEvent &arg);  // extended
    //virtual bool keyDoubleTap (const OIS::KeyEvent &arg);  // extended

    /// Gamepad Listeners
    virtual bool buttonPressed (const OIS::JoyStickEvent &arg, int button);  // basic
    virtual bool buttonReleased(const OIS::JoyStickEvent &arg, int button);  // basic
    virtual bool axisMoved     (const OIS::JoyStickEvent &arg, int axis);    // basic
    virtual bool sliderMoved (const OIS::JoyStickEvent &arg, int index);  // advanced
    virtual bool povMoved    (const OIS::JoyStickEvent &arg, int index);  // advanced
    virtual bool vector3Moved(const OIS::JoyStickEvent &arg, int index);  // advanced

    void loadMouseEvent(const OIS::MouseEvent &arg);
    void loadKeyEvent(const OIS::KeyEvent &arg);
    JoyStickEvent* loadJoyStickEvent(const OIS::JoyStickEvent &arg, InputType mod, int index);

    RenderWindow *mWindow;
    OIS::InputManager *mInputManager;
    OIS::Mouse    *mMouse;
    OIS::Keyboard *mKeyboard;
    JoyStickVector mJoysticks;

    std::list<Caelum::KeyListener*> mKeyListeners;
    std::list<Caelum::MouseListener*> mMouseListeners;
    std::list<Caelum::JoyStickListener*> mJoyListeners;
    Caelum::KeyEvent mKeyEvent;
    Caelum::MouseEvent mMouseEvent;
    JoyStickEventMap mJoyStickEvents;
    //Ogre::Timer *mMouseTimer[NUM_MOUSE_BUTTONS];
};

}  // namespace NAGE

#endif  // SRC_INPUT_INPUTMANAGER_H_
