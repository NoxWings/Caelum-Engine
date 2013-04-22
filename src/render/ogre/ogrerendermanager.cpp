#include "ogrerendermanager.h"

using namespace Caelum;

OgreRenderManager::OgreRenderManager() {
    mRoot = Ogre::Root::getSingletonPtr();
    mRoot->addFrameListener(this);
}

OgreRenderManager::~OgreRenderManager() {
    mRoot->removeFrameListener(this);
    mRoot = 0;
}

void OgreRenderManager::startRendering() {
    mIsRenderLoopActive = true;
    mRoot->startRendering();
    mIsRenderLoopActive = false;
}


// ************************************
// Ogre Frame Event Listener Interface
// ************************************

bool OgreRenderManager::frameStarted(const Ogre::FrameEvent &evt) {
    // default action = continue
    bool cont = true;
    // remap the event from ogre to caelum
    RenderEvent revt;
    revt.timeSinceLastEvent = evt.timeSinceLastEvent;
    revt.timeSinceLastRender = evt.timeSinceLastFrame;
    // declare a lister var
    RenderListener* listener;
    // iterate and dispatch the event
    for (ItemList::iterator it = mItems.begin();
         it != mItems.end();
         ++it) {
        listener = (*it);
        if (!listener->preRenderUpdate(revt)) cont = false;
    }
    return cont;
}

bool OgreRenderManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    // default action = continue
    bool cont = true;
    // remap the event from ogre to caelum
    RenderEvent revt;
    revt.timeSinceLastEvent = evt.timeSinceLastEvent;
    revt.timeSinceLastRender = evt.timeSinceLastFrame;
    // declare a lister var
    RenderListener* listener;
    // iterate and dispatch the event
    for (ItemList::iterator it = mItems.begin();
         it != mItems.end();
         ++it) {
        listener = (*it);
        if (!listener->renderingUpdate(revt)) cont = false;
    }
    return cont;
}

bool OgreRenderManager::frameEnded(const Ogre::FrameEvent& evt) {
    // default action = continue
    bool cont = true;
    // remap the event from ogre to caelum
    RenderEvent revt;
    revt.timeSinceLastEvent = evt.timeSinceLastEvent;
    revt.timeSinceLastRender = evt.timeSinceLastFrame;
    // declare a lister var
    RenderListener* listener;
    // iterate and dispatch the event
    for (ItemList::iterator it = mItems.begin();
         it != mItems.end();
         ++it) {
        listener = (*it);
        if (!listener->postRenderUpdate(revt)) cont = false;
    }
    return cont;
}
