#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "EnginePrerequisites.h"

#include "patterns/Singleton.h"
#include "game/scene.h"

#include <map>
#include <list>

namespace MyGUI {
class OgrePlatform;
class Gui;
class Widget;
typedef std::vector<Widget*> VectorWidgetPtr;
}

namespace Caelum {

class GuiInstance;

class GUIManager
        : public Singleton<GUIManager> {
  public:
    typedef std::map<String, MyGUI::VectorWidgetPtr> LayoutMap;
  public:
    GUIManager(GuiInstance *instance);
    virtual ~GUIManager();

    static bool isCreated() { return ms_Singleton; }
    GuiInstance* getActiveInstance() {return mActiveInstance;}

    template <typename T>
    T* findWidget(const String &name, const String &prefix);
    template <typename T>
    T* findWidget(const String &name);

    void loadLayout(const String &layoutFile, const String &prefix = "");
    void unloadLayout(const String &layoutFile);
    void unloadAllLayouts();

    void setPointerVisible(bool visible);
    bool getPointerVisible();

    MyGUI::Gui *getGUI() { return mGUI;}

  protected:
    MyGUI::OgrePlatform *mPlatform;
    MyGUI::Gui *mGUI;
    GuiInstance *mActiveInstance;
    LayoutMap mLayouts;
};

class GuiInstance {
  public:
    typedef std::map<String, String> LayoutPrefixMap;
  public:
    GuiInstance(Scene *scene);
    ~GuiInstance();

    void resume();
    void pause();
    bool hasControl() { return mManager;}

    template <typename T>
    T* findWidget(const String &name, const String &prefix);
    template <typename T>
    T* findWidget(const String &name);

    void loadLayout(const String &layoutFile, const String &prefix);
    void unloadLayout(const String &layoutFile);

    void setPointerVisible(bool visible);
    bool getPointerVisible();

    Caelum::Scene *getScene() { return mScene;}
    MyGUI::Gui *getGUI() {return mManager->getGUI();}

  protected:
    LayoutPrefixMap mLayoutPrefix;
    Caelum::Scene* mScene;
    GUIManager* mManager;

};

}

#endif // GUIMANAGER_H
