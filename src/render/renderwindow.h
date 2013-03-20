#ifndef SRC_RENDER_RENDERWINDOW_H
#define SRC_RENDER_RENDERWINDOW_H

#include "EnginePrerequisites.h"

#include "patterns/ListenerRegistrant.h"

#include <map>

namespace Caelum {

/** Fordward declaration **/
class WindowListener;
typedef std::map<String, String> VideoOptions;

class RenderWindow  : public ListenerRegistrant<WindowListener>{
  public:
    RenderWindow();
    virtual ~RenderWindow();

    virtual void addViewPort() = 0;
};

}

#endif // RENDERWINDOW_H
