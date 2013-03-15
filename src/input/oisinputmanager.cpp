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
#include <OGRE/OgreWindowEventUtilities.h>

#include <list>
#include <string>


using namespace Caelum;

// Constructor
OISInputManager::OISInputManager()
    : mWindow(0), mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0) {
    setup();

    /* Note: Emergency shutdown of OIS just in case the destructor of this object
     * cannot be deactivated due to any crash of the game */
    atexit(OISInputManager::_emergency_shutdown);
}

// Destructor
OISInputManager::~InputManager() {
    shutdown();
}


/***********************************************
     UPDATE
************************************************/

void OISInputManager::update() {
    // Capturing Devices
    if (mMouse)    mMouse->capture();
    if (mKeyboard) mKeyboard->capture();
    if (mJoy)      mJoy->capture();
}



/***********************************************
     SETUP & SHUTDOWN
************************************************/

// Initialization Method
void OISInputManager::setup() {
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
void OISInputManager::shutdown() {
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

void OISInputManager::_emergency_shutdown() {
    if (ms_Singleton) OISInputManager::getSingletonPtr()->shutdown();
}

/***********************************************
     UTILITY
************************************************/

Ogre::RenderWindow* OISInputManager::getDefaultWindow() {
    return Ogre::Root::getSingletonPtr()->getAutoCreatedWindow();
}

void OISInputManager::registerRenderListeners() {
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);  // Window listener
    Ogre::Root::getSingletonPtr()->addFrameListener(this);  // FrameListener
}

void OISInputManager::unregisterRenderListeners() {
    Ogre::Root::getSingletonPtr()->removeFrameListener(this);
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
}

/***********************************************
     INPUT SYSTEM
************************************************/

OIS::InputManager* OISInputManager::createInputSystem() {
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

OIS::Mouse* OISInputManager::createMouse(bool buffered) {
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

OIS::Keyboard* OISInputManager::createKeyboard(bool buffered) {
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

OIS::JoyStick* OISInputManager::createJoyStick(bool buffered) {
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

void OISInputManager::destroyInputSystem() {
    OIS::InputManager::destroyInputSystem(mInputManager);
    mInputManager = NULL;
}

void OISInputManager::destroyMouse() {
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

void OISInputManager::destroyKeyBoard() {
    if (mKeyboard) {
        mKeyListeners.clear();
        mInputManager->destroyInputObject(mKeyboard);
    }
    mKeyboard = NULL;
}

void OISInputManager::destroyJoyStick() {
    if (mJoy) {
        mJoyListeners.clear();
        mInputManager->destroyInputObject(mJoy);
    }
    mJoy = NULL;
}


/***********************************************
     WINDOW LISTENERS
************************************************/

void OISInputManager::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// Destroy OIS before window shutdown (very important under Linux)
void OISInputManager::windowClosed(Ogre::RenderWindow* rw) {
    if (rw == mWindow)
        shutdown();
}


/***********************************************
     RENDERING LISTENERS
************************************************/

bool OISInputManager::frameStarted(const Ogre::FrameEvent &evt) {
    // TODO  (check this! dont think the next line is actually necessary)
    if (mWindow->isClosed()) return false;  // Shutdown Every Render
    update();
    return true;
}

bool OISInputManager::frameEnded(const Ogre::FrameEvent &evt) {
    return true;
}


/***********************************************
     MOUSE LISTENERS
************************************************/

bool OISInputManager::mouseMoved(const OIS::MouseEvent &arg) {
    // Populate Event
    std::list<OIS::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mouseMoved(arg);
        iter++;
    }
    return true;
}

bool OISInputManager::mousePressed(const OIS::MouseEvent &arg,
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

bool OISInputManager::mouseReleased(const OIS::MouseEvent &arg,
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

bool OISInputManager::mouseClick(const OIS::MouseEvent &arg,
                              OIS::MouseButtonID id) {
    // TODO
    /* not implemented yet */
    return true;
}

bool OISInputManager::mouseDoubleClick(const OIS::MouseEvent &arg,
                                    OIS::MouseButtonID id) {
    // TODO
    /* not implemented yet */
    return true;
}


/***********************************************
     KEYBOARD LISTENERS
************************************************/

bool OISInputManager::keyPressed(const OIS::KeyEvent &arg) {
    // Populate Event
    std::list<OIS::KeyListener*>::iterator iter = mKeyListeners.begin();
    while (iter != mKeyListeners.end()) {
        (*iter)->keyPressed(arg);
        iter++;
    }
    return true;
}

bool OISInputManager::keyReleased(const OIS::KeyEvent &arg) {
    // Populate Event
    std::list<OIS::KeyListener*>::iterator iter = mKeyListeners.begin();
    while (iter != mKeyListeners.end()) {
        (*iter)->keyReleased(arg);
        iter++;
    }
    return true;
}

bool OISInputManager::keyTap(const OIS::KeyEvent &arg) {
    // TODO
    /* not implemented yet */
    return true;
}

bool OISInputManager::keyDoubleTap(const OIS::KeyEvent &arg) {
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
