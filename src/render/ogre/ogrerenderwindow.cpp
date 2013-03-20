#include "render/ogre/ogrerenderwindow.h"

#include <OGRE/Ogre.h>
#include "render/windowlistener.h"

using namespace Caelum;

OgreRenderWindow::OgreRenderWindow(const String& windowName,
                                   unsigned int resolutionW,
                                   unsigned int resolutionH,
                                   bool fullscreen,
                                   VideoOptions* videoOpts) {
    // Conversion from VideoOptions to NameValuePairList
    // implicit conversion is not posible due to NameValuePairList has custom allocators
    Ogre::NameValuePairList nvp;
    for (VideoOptions::iterator it = videoOpts->begin();
         it != videoOpts->end();
         ++it) {
        nvp.insert(Ogre::NameValuePairList::value_type(it->first, it->second));
    }

    // Create the ogre window
    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    mRenderWindow = root->createRenderWindow(windowName,
                                             resolutionW, resolutionH,
                                             fullscreen,
                                             &nvp );

    // Add this as a window listener
    // later we will be dispatching every event to our own listeners
    Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
}

OgreRenderWindow::~OgreRenderWindow() {
    // Remove ourselves as a listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mRenderWindow, this);

    // Destroy the render window
    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    root->destroyRenderTarget(mRenderWindow);
}

void OgreRenderWindow::addViewPort() {
    //mRenderWindow->addViewport();
}

// ************************************
// Ogre Window Event Listener Interface
// ************************************

void OgreRenderWindow::windowMoved(Ogre::RenderWindow *rw) {
    if (mRenderWindow == rw) {
        for (ListenerList::iterator it = mListeners.begin();
             it != mListeners.end();
             ++it) {
            // here goes the dispatch
            WindowListener* listener = (*it);
            listener->windowMoved(this);
        }
    }
}

void OgreRenderWindow::windowResized(Ogre::RenderWindow *rw) {
    if (mRenderWindow == rw) {
        for (ListenerList::iterator it = mListeners.begin();
             it != mListeners.end();
             ++it) {
            // here goes the dispatch
            WindowListener* listener = (*it);
            listener->windowResized(this);
        }
    }
}

bool OgreRenderWindow::windowClosing(Ogre::RenderWindow *rw) {
    bool res = false;
    if (mRenderWindow == rw) {
        for (ListenerList::iterator it = mListeners.begin();
             it != mListeners.end();
             ++it) {
            // here goes the dispatch
            WindowListener* listener = (*it);
            if (listener->windowClosing(this)) res = true;
        }
    }
    return res;
}

void OgreRenderWindow::windowClosed(Ogre::RenderWindow *rw) {
    if (mRenderWindow == rw) {
        for (ListenerList::iterator it = mListeners.begin();
             it != mListeners.end();
             ++it) {
            // here goes the dispatch
            WindowListener* listener = (*it);
            listener->windowClosed(this);
        }
    }
}

void OgreRenderWindow::windowFocusChange(Ogre::RenderWindow *rw) {
    if (mRenderWindow == rw) {
        for (ListenerList::iterator it = mListeners.begin();
             it != mListeners.end();
             ++it) {
            // here goes the dispatch
            WindowListener* listener = (*it);
            listener->windowFocusChange(this);
        }
    }
}
