/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

// Input Header
#include "input/inputmanager.h"
// Ogre
#include <OGRE/OgreWindowEventUtilities.h>

#include <list>
#include <string>


using namespace Caelum;

// Constructor
InputManager::InputManager()
    : mWindow(0), mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0) {
    setup();

    /* Note: Emergency shutdown of OIS just in case the destructor of this object
     * cannot be deactivated due to any crash of the game */
    atexit(InputManager::_emergency_shutdown);
}

// Destructor
InputManager::~InputManager() {
    shutdown();
}


/***********************************************
     SETUP & SHUTDOWN
************************************************/

// Initialization Method
void InputManager::setup() {
    // 1. Window
    mWindow = getDefaultWindow();
    assert(mWindow);  // This windows must have been created

    // 2. Input Manager
    createInputSystem();

    // 3. Devices        (Mouse, Keyboard, Joystick)
    bool buffered = true;
    createMouse(buffered);
    createKeyboard(buffered);
    createJoyStick(buffered);

    // 4. LISTENERS
    registerRenderListeners();
}

// Shutdown Method
void InputManager::shutdown() {
    if (mInputManager) {
        // 4. Remove listeners
        unregisterRenderListeners();

        // 3. Remove the OIS devices
        destroyJoyStick();
        destroyKeyBoard();
        destroyMouse();

        // 2. Destroy OIS InputManager
        destroyInputSystem();

        // 1. Window dereference
        mWindow = NULL;
    }
}

void InputManager::_emergency_shutdown() {
    if (ms_Singleton) InputManager::getSingletonPtr()->shutdown();
}

/***********************************************
     UTILITY
************************************************/

Ogre::RenderWindow* InputManager::getDefaultWindow() {
    return Ogre::Root::getSingletonPtr()->getAutoCreatedWindow();
}

void InputManager::registerRenderListeners() {
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);  // Window listener
    Ogre::Root::getSingletonPtr()->addFrameListener(this);  // FrameListener
}

void InputManager::unregisterRenderListeners() {
    Ogre::Root::getSingletonPtr()->removeFrameListener(this);
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
}

/***********************************************
     INPUT SYSTEM
************************************************/

OIS::InputManager* InputManager::createInputSystem() {
    if (!mInputManager) {
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        OIS::ParamList pl;
        mWindow->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(pl);
    }
    return mInputManager;
}

OIS::Mouse* InputManager::createMouse(bool buffered) {
    if (!mMouse) {
        try {
            // 1. Create device
            mMouse = static_cast<OIS::Mouse*>(
                        mInputManager->createInputObject(OIS::OISMouse,
                                                         buffered));
            // 2. Initialize mouse timers
            for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
                mMouseTimer[i] = new Ogre::Timer();
            }
            // 3. Set event callback
            mMouse->setEventCallback(this);

            // 4. Set initial mouse clipping size
            windowResized(mWindow);
        } catch (...) {
            mMouse = NULL;
        }
    }
    return mMouse;
}

OIS::Keyboard* InputManager::createKeyboard(bool buffered) {
    if (!mKeyboard) {
        try {
            // 1. Create device
            mKeyboard = static_cast<OIS::Keyboard*>(
                        mInputManager->createInputObject(OIS::OISKeyboard,
                                                         buffered));
            // 2. Set event callback
            mKeyboard->setEventCallback(this);
        } catch (...) {
            mKeyboard = NULL;
        }
    }
    return mKeyboard;
}

OIS::JoyStick* InputManager::createJoyStick(bool buffered) {
    if (!mJoy) {
        try {
            // 1. Create device
            mJoy = static_cast<OIS::JoyStick*>(
                        mInputManager->createInputObject(OIS::OISJoyStick,
                                                         buffered));
            // 2. Set event callback
            mJoy->setEventCallback(this);
        } catch (...) {
            mJoy = NULL;
        }
    }
    return mJoy;
}

void InputManager::destroyInputSystem() {
    OIS::InputManager::destroyInputSystem(mInputManager);
    mInputManager = NULL;
}

void InputManager::destroyMouse() {
    if (mMouse) {
        mMouseListeners.clear();
        mInputManager->destroyInputObject(mMouse);

        // Delete timers
        for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
            delete mMouseTimer[i];
        }
    }
    mMouse = NULL;
}

void InputManager::destroyKeyBoard() {
    if (mKeyboard) {
        mKeyListeners.clear();
        mInputManager->destroyInputObject(mKeyboard);
    }
    mKeyboard = NULL;
}

void InputManager::destroyJoyStick() {
    if (mJoy) {
        mJoyListeners.clear();
        mInputManager->destroyInputObject(mJoy);
    }
    mJoy = NULL;
}


/***********************************************
     UPDATE
************************************************/

void InputManager::update() {
    // Capturing Devices
    if (mMouse)    mMouse->capture();
    if (mKeyboard) mKeyboard->capture();
    if (mJoy)      mJoy->capture();
}


/***********************************************
     WINDOW LISTENERS
************************************************/

void InputManager::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// Destroy OIS before window shutdown (very important under Linux)
void InputManager::windowClosed(Ogre::RenderWindow* rw) {
    if (rw == mWindow)
        shutdown();
}


/***********************************************
     RENDERING LISTENERS
************************************************/

bool InputManager::frameStarted(const Ogre::FrameEvent &evt) {
    // TODO  (check this! dont think the next line is actually necessary)
    if (mWindow->isClosed()) return false;  // Shutdown Every Render
    update();
    return true;
}

bool InputManager::frameEnded(const Ogre::FrameEvent &evt) {
    return true;
}


/***********************************************
     MOUSE LISTENERS
************************************************/

bool InputManager::mouseMoved(const OIS::MouseEvent &arg) {
    // Populate Event
    std::list<OIS::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mouseMoved(arg);
        iter++;
    }
    return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &arg,
                                OIS::MouseButtonID id) {
    // Reset the specific timer id
    mMouseTimer[id]->reset();

    // Populate Event
    std::list<OIS::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mousePressed(arg, id);
        iter++;
    }
    return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &arg,
                                 OIS::MouseButtonID id) {
    // Populate Event
    std::list<OIS::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mouseReleased(arg, id);
        iter++;
    }

    // TODO
    return true;
}

bool InputManager::mouseClick(const OIS::MouseEvent &arg,
                              OIS::MouseButtonID id) {
    // TODO
    /* not implemented yet */
    return true;
}

bool InputManager::mouseDoubleClick(const OIS::MouseEvent &arg,
                                    OIS::MouseButtonID id) {
    // TODO
    /* not implemented yet */
    return true;
}


/***********************************************
     KEYBOARD LISTENERS
************************************************/

bool InputManager::keyPressed(const OIS::KeyEvent &arg) {
    // Populate Event
    std::list<OIS::KeyListener*>::iterator iter = mKeyListeners.begin();
    while (iter != mKeyListeners.end()) {
        (*iter)->keyPressed(arg);
        iter++;
    }
    return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &arg) {
    // Populate Event
    std::list<OIS::KeyListener*>::iterator iter = mKeyListeners.begin();
    while (iter != mKeyListeners.end()) {
        (*iter)->keyReleased(arg);
        iter++;
    }
    return true;
}

bool InputManager::keyTap(const OIS::KeyEvent &arg) {
    // TODO
    /* not implemented yet */
    return true;
}

bool InputManager::keyDoubleTap(const OIS::KeyEvent &arg) {
    // TODO
    /* not implemented yet */
    return true;
}


/***********************************************
     JOYSTICK LISTENERS
************************************************/

bool InputManager::buttonPressed(const OIS::JoyStickEvent &arg, int button) {
    // TODO
    /* not implemented yet */
    return true;
}

bool InputManager::buttonReleased(const OIS::JoyStickEvent &arg, int button) {
    // TODO
    /* not implemented yet */
    return true;
}

bool InputManager::axisMoved(const OIS::JoyStickEvent &arg, int axis) {
    // TODO
    /* not implemented yet */
    return true;
}
