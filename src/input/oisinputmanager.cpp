/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

// Input Header
#include "input/oisinputmanager.h"

// Ogre
#include <OGRE/OgreRenderWindow.h>


using namespace Caelum;

// Constructor
OISInputManager::OISInputManager()
    : mWindow(0), mInputManager(0), mMouse(0), mKeyboard(0) {
    setup();
    /* Note: Emergency shutdown of OIS just in case the destructor of this object
     * cannot be deactivated due to any crash of the game */
    atexit(OISInputManager::_emergency_shutdown);
}

// Destructor
OISInputManager::~OISInputManager() {
    shutdown();
}


/***********************************************
     UPDATE
************************************************/

void OISInputManager::update() {
    // Capturing Devices
    if (mMouse)    mMouse->capture();
    if (mKeyboard) mKeyboard->capture();
    //if (mJoysticks.size() > 0) {
        //mJoy->capture();
    //}
}

void OISInputManager::windowResized(RenderWindow *rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getActualWindow()->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void OISInputManager::windowClosed(RenderWindow *rw) {
    if (rw == mWindow) {
        this->shutdown();
    }
}

/***********************************************
     SETUP & SHUTDOWN
************************************************/

// Initialization Method
void OISInputManager::setup() {
    // 1. Window
    mWindow = RenderManager::getSingletonPtr()->getRenderWindow();
    assert(mWindow);  // This window must have been created
    // 2. Input Manager
    createInputSystem(true);
    // 3. Devices        (Mouse, Keyboard, Joysticks)
    bool buffered = true;
    createMouse(buffered);
    createKeyboard(buffered);
    //createJoySticks(buffered);
    // 4.- Register as a window listener
    RenderManager::getSingletonPtr()->getRenderWindow()->addListener(this);
}

// Shutdown Method
void OISInputManager::shutdown() {
    if (mInputManager) {
        // 4.- Remove as a window listener
        RenderManager::getSingletonPtr()->getRenderWindow()->removeListener(this);
        // 3. Remove the OIS devices
        //destroyJoySticks();
        destroyKeyBoard();
        destroyMouse();
        // 2. Destroy OIS InputManager
        destroyInputSystem();
        // 1. Window dereference
        mWindow = NULL;
    }
}

void OISInputManager::_emergency_shutdown() {
    //if (ms_Singleton) static_cast<OISInputManager>(InputManager::getSingleton()).shutdown();
}


/***********************************************
     INPUT SYSTEM
************************************************/

OIS::InputManager* OISInputManager::createInputSystem(bool grabInput) {
    if (!mInputManager) {
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        OIS::ParamList pl;
        mWindow->getActualWindow()->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

        // Mouse Grabbing prevention Workaround
        if (!grabInput) {
#if defined OIS_WIN32_PLATFORM
        pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
        pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
        pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
        pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
        pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
        pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
        }
        mInputManager = OIS::InputManager::createInputSystem(pl);
    }
    return mInputManager;
}

OIS::Mouse* OISInputManager::createMouse(bool buffered) {
    if (!mMouse) {
        try {
            // 1. Create device
            mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, buffered));
            // 2. Initialize mouse timers
            /*for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
                mMouseTimer[i] = new Ogre::Timer();
            }*/
            // 3. Set event callback
            mMouse->setEventCallback(this);
            // 4. Set initial mouse clipping size
            this->windowResized(mWindow);
        } catch (...) {
            mMouse = NULL;
        }
    }
    return mMouse;
}

OIS::Keyboard* OISInputManager::createKeyboard(bool buffered) {
    if (!mKeyboard) {
        try {
            // 1. Create device
            mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, buffered));
            // 2. Set event callback
            mKeyboard->setEventCallback(this);
        } catch (...) {
            mKeyboard = NULL;
        }
    }
    return mKeyboard;
}

/*
const OISInputManager::JoyStickVector&
OISInputManager::createJoySticks(bool buffered) {
    int numJoysticks = mInputManager->getNumberOfDevices(OIS::OISJoyStick);
    if (numJoysticks > 0) {
        mJoysticks.resize(numJoysticks);
        for(JoyStickVector::iterator it = mJoysticks.begin();
            it != mJoysticks.end();
            ++it) {
            // 1. Create device
            (*it) = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, buffered));
            // 2. Set event callback
            (*it)->setEventCallback(this);
        }
    }
    return mJoysticks;
}
*/

void OISInputManager::destroyInputSystem() {
    OIS::InputManager::destroyInputSystem(mInputManager);
    mInputManager = NULL;
}

void OISInputManager::destroyMouse() {
    if (mMouse) {
        mMouseListeners.clear();
        mInputManager->destroyInputObject(mMouse);
        // Delete timers
        /*for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
            delete mMouseTimer[i];
        }*/
    }
    mMouse = NULL;
}

void OISInputManager::destroyKeyBoard() {
    if (mKeyboard) {
        mKeyListeners.clear();
        mInputManager->destroyInputObject(mKeyboard);
    }
    mKeyboard = NULL;
}

/*
void OISInputManager::destroyJoySticks() {
    if (!mJoysticks.empty()) {
        //mJoyListeners.clear();
        for (JoyStickVector::iterator it = mJoysticks.begin();
             it != mJoysticks.end();
             ++it) {
            mInputManager->destroyInputObject((*it));
        }
        mJoysticks.clear();
    }
}
*/


/***********************************************
     MOUSE LISTENERS
************************************************/

bool OISInputManager::mouseMoved(const OIS::MouseEvent &arg) {
    loadMouseEvent(arg);
    // Populate Event
    std::list<Caelum::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mouseMoved(mMouseEvent);
        iter++;
    }
    return true;
}

bool OISInputManager::mousePressed(const OIS::MouseEvent &arg,
                                OIS::MouseButtonID id) {
    loadMouseEvent(arg);
    // Populate Event
    std::list<Caelum::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mousePressed(mMouseEvent, Caelum::MouseButtonID(id));
        iter++;
    }
    return true;
}

bool OISInputManager::mouseReleased(const OIS::MouseEvent &arg,
                                 OIS::MouseButtonID id) {
    loadMouseEvent(arg);
    // Populate Event
    std::list<Caelum::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mouseReleased(mMouseEvent, Caelum::MouseButtonID(id));
        iter++;
    }
    return true;
}

bool OISInputManager::mouseClick(const OIS::MouseEvent &arg,
                              OIS::MouseButtonID id) {
    // TODO
    /* not implemented yet */
    return true;
}


/***********************************************
     KEYBOARD LISTENERS
************************************************/

bool OISInputManager::keyPressed(const OIS::KeyEvent &arg) {
    loadKeyEvent(arg);
    // Populate Event
    std::list<Caelum::KeyListener*>::iterator iter = mKeyListeners.begin();
    while (iter != mKeyListeners.end()) {
        (*iter)->keyPressed(mKeyEvent);
        iter++;
    }
    return true;
}

bool OISInputManager::keyReleased(const OIS::KeyEvent &arg) {
    loadKeyEvent(arg);
    // Populate Event
    std::list<Caelum::KeyListener*>::iterator iter = mKeyListeners.begin();
    while (iter != mKeyListeners.end()) {
        (*iter)->keyReleased(mKeyEvent);
        iter++;
    }
    return true;
}

bool OISInputManager::keyTap(const OIS::KeyEvent &arg) {
    // TODO
    /* not implemented yet */
    return true;
}


/***********************************************
     JOYSTICK LISTENERS
************************************************/

bool OISInputManager::buttonPressed(const OIS::JoyStickEvent &arg, int button) {
    // TODO
    /* not implemented yet */
    return true;
}

bool OISInputManager::buttonReleased(const OIS::JoyStickEvent &arg, int button) {
    // TODO
    /* not implemented yet */
    return true;
}

bool OISInputManager::axisMoved(const OIS::JoyStickEvent &arg, int axis) {
    // TODO
    /* not implemented yet */
    return true;
}


/***********************************************
     EVENT CONVERSION
************************************************/

void OISInputManager::loadMouseEvent(const OIS::MouseEvent &arg) {
    // buttons
    mMouseEvent.buttons = arg.state.buttons;
    // Window size
    mMouseEvent.width = arg.state.width; mMouseEvent.height = arg.state.height;
    // X, Y, Z axis
    mMouseEvent.X.absOnly = arg.state.X.absOnly; mMouseEvent.X.abs = arg.state.X.abs; mMouseEvent.X.rel = arg.state.X.rel;
    mMouseEvent.Y.absOnly = arg.state.Y.absOnly; mMouseEvent.Y.abs = arg.state.Y.abs; mMouseEvent.Y.rel = arg.state.Y.rel;
    mMouseEvent.Z.absOnly = arg.state.Z.absOnly; mMouseEvent.Z.abs = arg.state.Z.abs; mMouseEvent.Z.rel = arg.state.Z.rel;
}

void OISInputManager::loadKeyEvent(const OIS::KeyEvent &arg) {
    mKeyEvent.key = Caelum::KeyCode(arg.key);
    mKeyEvent.text = arg.text;
}

void OISInputManager::loadJoystickEvent(const OIS::JoyStickEvent &arg) {
}
