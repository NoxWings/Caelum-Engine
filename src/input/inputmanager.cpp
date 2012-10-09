/*
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  This file is part of NAGE.
 */

// Input Header
#include "input/inputmanager.hpp"
// Ogre
#include <OGRE/OgreWindowEventUtilities.h>

using namespace NAGE;

// Constructor
InputManager::InputManager()
    : mWindow(0), mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0) {
    setup();

    /* Note: Emergency shutdown of OIS just in case the destructor of this object cannot be deactivated
    due to any crash of the game */
    atexit(InputManager::_emergency_shutdown);
}

// Destructor
InputManager::~InputManager() {
    this->shutdown();
}


// Initialization Method
void InputManager::setup() {
    using namespace OIS;
    using namespace Ogre;

    // 1. Window
    mWindow = Ogre::Root::getSingletonPtr()->getAutoCreatedWindow();
    // TODO    (check that this window is effectively created)
    assert(mWindow);

    // 2. Input Manager
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    ParamList pl;
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    // 3. Devices        (Mouse, Keyboard, Joystick)
    bool bufferedKeys, bufferedMouse, bufferedJoy;
    bufferedKeys = bufferedMouse = bufferedJoy = true;
    mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, bufferedKeys )); // keyboard
    mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, bufferedMouse )); // mouse
    try {
        mJoy = static_cast<JoyStick*>(mInputManager->createInputObject( OISJoyStick, bufferedJoy )); // joystick
    } catch(...) {
        mJoy = 0;
    }
    windowResized(mWindow); // Set initial mouse clipping size


    // 4. LISTENERS & Callbacks
    WindowEventUtilities::addWindowEventListener(mWindow, this); // Window listener
    Ogre::Root::getSingletonPtr()->addFrameListener(this); // FrameListener
    if (mKeyboard) mKeyboard->setEventCallback(this);
    if (mMouse) mMouse->setEventCallback(this);
    if (mJoy) mJoy->setEventCallback(this);
}


// Shutdown Method
void InputManager::shutdown() {
    if (mInputManager) {
        // 4. Unregister suscribed listeners
        mKeyListeners.clear();
        mMouseListeners.clear();

        // 4. Unregister own listeners
        Ogre::Root *mRoot = Ogre::Root::getSingletonPtr();
        mRoot->removeFrameListener(this); // remove as a frame listener
        if (mWindow)
            Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

        // 3. Remove the OIS devices
        if (mMouse) mInputManager->destroyInputObject( mMouse );
        if (mKeyboard) mInputManager->destroyInputObject( mKeyboard );
        if (mJoy) mInputManager->destroyInputObject( mJoy );

        // 2. Destroy OIS InputManager
        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = 0;

        // 1. Window dereference
        mWindow = 0;
    }
}


// update
void InputManager::update() {
    // Capturing Devices
    if (mMouse) mMouse->capture();
    if (mKeyboard) mKeyboard->capture();
    if (mJoy) mJoy->capture();
}


/***************************
  WINDOW LISTENERS
***************************/

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

/***************************
  RENDERING LISTENERS
***************************/

bool InputManager::frameStarted(const Ogre::FrameEvent &evt) {
    // TODO  (check this! dont think the next line is actually necessary)
    if (mWindow->isClosed()) return false; // Shutdown Every Render
    update();
    return true;
}

bool InputManager::frameEnded(const Ogre::FrameEvent &evt) {
    return true;
}

/**************************
  INPUT LISTENERS (OIS)
***************************/


bool InputManager::keyPressed(const OIS::KeyEvent &arg) {
    // Populate Event
    std::list<OIS::KeyListener*>::iterator iter = mKeyListeners.begin();
    while (iter != mKeyListeners.end())
    {
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

bool InputManager::mouseMoved(const OIS::MouseEvent &arg) {
    // Populate Event
    std::list<OIS::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mouseMoved(arg);
        iter++;
    }
    return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // Populate Event
    std::list<OIS::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mousePressed(arg,id);
        iter++;
    }
    return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // Populate Event
    std::list<OIS::MouseListener*>::iterator iter = mMouseListeners.begin();
    while (iter != mMouseListeners.end()) {
        (*iter)->mouseReleased(arg,id);
        iter++;
    }
    return true;
}

bool InputManager::mouseClick(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // TODO
    /* not implemented yet */
    return true;
}

bool InputManager::mouseDoubleClick(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // TODO
    /* not implemented yet */
    return true;
}
