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

// OIS
// Use this define to signify OIS will be used as a DLL
// (so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

// Lib
#include "EnginePrerequisites.h"

// Patterns
#include "patterns/Singleton.h"

// Ogre
#include <OGRE/Ogre.h>
#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreTimer.h>

// C++
#include <list>
#include <map>

#define DEFAULT_CLICK_INTERVAL       250
#define DEFAULT_DOUBLECLICK_INTERVAL 300
#define DEFAULT_TAP_INTERVAL         250  // Time in miliseconds
#define DEFAULT_DOUBLETAP_INTERVAL   300  // Time in miliseconds


namespace Caelum {

/**
  */
class InputManager : public Singleton<InputManager>,  // Inheritance
        // Listener Interfaces
        public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener,
        public Ogre::WindowEventListener, public Ogre::FrameListener {
  public:
    InputManager();
    ~InputManager();

    void addKeyListener      (OIS::KeyListener *keyListener)     {mKeyListeners.push_back(keyListener);}
    void addMouseListener    (OIS::MouseListener *mouseListener) {mMouseListeners.push_back(mouseListener);}
    void removeKeyListener   (OIS::KeyListener *keyListener)     {mKeyListeners.remove(keyListener);}
    void removeMouseListener (OIS::MouseListener *mouseListener) {mMouseListeners.remove(mouseListener);}

  protected:
    /// SETUP & SHUTDOWN
    void setup();
    void shutdown();
    // fallback emergency method
    static inline void _emergency_shutdown();

    /// Utility
    Ogre::RenderWindow* getDefaultWindow();
    void registerRenderListeners();
    void unregisterRenderListeners();

    /// Input system
    OIS::InputManager* createInputSystem();
    OIS::Mouse*    createMouse   (bool buffered);
    OIS::Keyboard* createKeyboard(bool buffered);
    OIS::JoyStick* createJoyStick(bool buffered);
    void destroyInputSystem();
    void destroyMouse();
    void destroyKeyBoard();
    void destroyJoyStick();

    /// Update
    virtual void update();

    /// Window Event Listener
    virtual void windowResized(Ogre::RenderWindow *rw);
    virtual void windowClosed (Ogre::RenderWindow *rw);

    /// Render Listeners
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual bool frameEnded  (const Ogre::FrameEvent &evt);

    /// Input Listeners
    virtual bool mouseMoved       (const OIS::MouseEvent &arg);  // basic
    virtual bool mousePressed     (const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // basic
    virtual bool mouseReleased    (const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // basic
    virtual bool mouseClick       (const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // extended
    virtual bool mouseDoubleClick (const OIS::MouseEvent &arg, OIS::MouseButtonID id);  // extended

    virtual bool keyPressed   (const OIS::KeyEvent &arg);  // basic
    virtual bool keyReleased  (const OIS::KeyEvent &arg);  // basic
    virtual bool keyTap       (const OIS::KeyEvent &arg);  // extended
    virtual bool keyDoubleTap (const OIS::KeyEvent &arg);  // extended

    virtual bool buttonPressed  (const OIS::JoyStickEvent &arg, int button);  // basic
    virtual bool buttonReleased (const OIS::JoyStickEvent &arg, int button);  // basic
    virtual bool axisMoved      (const OIS::JoyStickEvent &arg, int axis);    // basic


    Ogre::RenderWindow *mWindow;
    OIS::InputManager *mInputManager;
    OIS::Mouse    *mMouse;
    OIS::Keyboard *mKeyboard;
    OIS::JoyStick *mJoy;

    std::list<OIS::KeyListener*> mKeyListeners;
    std::list<OIS::MouseListener*> mMouseListeners;
    std::list<OIS::JoyStickListener*> mJoyListeners;

    enum MouseButtonID {
        MB_Left = 0, MB_Right, MB_Middle,
        MB_Button3, MB_Button4, MB_Button5, MB_Button6, MB_Button7,
        NUM_MOUSE_BUTTONS
    };
    Ogre::Timer *mMouseTimer[NUM_MOUSE_BUTTONS];
};
}  // namespace NAGE

#endif  // SRC_INPUT_INPUTMANAGER_H_
