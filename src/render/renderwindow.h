/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_RENDER_RENDERWINDOW_H
#define SRC_RENDER_RENDERWINDOW_H

#include "EnginePrerequisites.h"

#include "patterns/SimpleCollection.h"

#include <map>

namespace Ogre {
class RenderWindow;
}

namespace Caelum {

/** Fordward declaration **/
class WindowListener;
class CameraComponent;
typedef std::map<String, String> VideoOptions;

class RenderWindow  : public SimpleCollection<WindowListener*>{
  public:
    RenderWindow();
    virtual ~RenderWindow();

    void addListener(WindowListener* listener) {this->addItem(listener);}
    void removeListener(WindowListener* listener) {this->removeItem(listener);}

    virtual void setViewportCamera(CameraComponent *camera) = 0;

    virtual Ogre::RenderWindow* getActualWindow() = 0;
};

}

#endif // RENDERWINDOW_H
