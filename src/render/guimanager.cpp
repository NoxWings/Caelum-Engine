#include "render/guimanager.h"

#include <MyGUI/MyGUI.h>
#include <MyGUIOgrePlatform/MyGUI_OgrePlatform.h>
#include "render/rendermanager.h"
#include "core/logmanager.h"

using namespace Caelum;

GUIManager::GUIManager(GuiInstance *instance)
    : mActiveInstance(instance) {
    mPlatform = new MyGUI::OgrePlatform();
    mPlatform->initialise(Caelum::RenderManager::getSingletonPtr()->getRenderWindow()->getActualWindow(),
                          mActiveInstance->getScene()->getSceneMgr(),
                          Caelum::ResourceManager::AUTODETECT_RESOURCE_GROUP_NAME,
                          "gui.log");
    mGUI = new MyGUI::Gui();
    mGUI->initialise("GUI_Core.xml");
}

GUIManager::~GUIManager() {
    unloadAllLayouts();

    mGUI->shutdown();
    delete mGUI;
    mGUI = 0;
    mPlatform->shutdown();
    delete mPlatform;
    mPlatform = 0;

    mActiveInstance = 0;
    /*LogManager::getSingleton().logMessage("GUI: Shutting down gui manager");
    LogManager::getSingleton().logMessage("GUI: Destroying gui manager");
    LogManager::getSingleton().logMessage("GUI: Shuting down gui platform connection");
    LogManager::getSingleton().logMessage("GUI: Destroying platform connection");
    LogManager::getSingleton().logMessage("GUI: Completely destroyed");*/
}

/*void GUIManager::setActiveScene(Caelum::Scene *scene) {
    mScene = scene;
    LogManager::getSingleton().logMessage("Activating scene gui");
    mPlatform->getRenderManagerPtr()->setSceneManager(mScene->getSceneMgr());
    LogManager::getSingleton().logMessage("Activating window gui");
    mPlatform->getRenderManagerPtr()->setRenderWindow(RenderManager::getSingletonPtr()->getRenderWindow()->getActualWindow());
    LogManager::getSingleton().logMessage("Activating viewport gui");
    mPlatform->getRenderManagerPtr()->setActiveViewport(0);
}*/

template <typename T>
T* GUIManager::findWidget(const String &name) {
    return mGUI->findWidget<T>(name, false);
}

template <typename T>
T* GUIManager::findWidget(const String &name, const String &prefix) {
    return mGUI->findWidget<T>(name, prefix, false);
}

void GUIManager::loadLayout(const String &layoutFile, const String &prefix) {
    mLayouts[layoutFile] =  MyGUI::LayoutManager::getInstancePtr()->loadLayout(layoutFile, prefix);
}

void GUIManager::unloadLayout(const String &layoutFile) {
    LayoutMap::iterator it = mLayouts.find(layoutFile);
    if (it != mLayouts.end()) {
        MyGUI::LayoutManager::getInstancePtr()->unloadLayout(it->second);
        mLayouts.erase(it);
    } else {
        // LAYOUT NOT FOUND
    }
}

void GUIManager::unloadAllLayouts() {
    for (LayoutMap::iterator it = mLayouts.begin();
         it != mLayouts.end();
         ++it) {
        MyGUI::LayoutManager::getInstancePtr()->unloadLayout(it->second);
    }
    mLayouts.clear();
}

void GUIManager::setPointerVisible(bool visible) {
    MyGUI::PointerManager::getInstancePtr()->setVisible(visible);
}

bool GUIManager::getPointerVisible() {
    return MyGUI::PointerManager::getInstancePtr()->isVisible();
}






/*****************************************************************************/
//                     GuiInstance
/*****************************************************************************/


GuiInstance::GuiInstance(Scene *scene)
    : mScene(scene), mManager(0) {
}

GuiInstance::~GuiInstance() {
    pause();
    mLayoutPrefix.clear();
    mScene = 0;
}

void GuiInstance::resume() {
    // find old owner
    if (GUIManager::isCreated()) {
        GuiInstance *instance = GUIManager::getSingleton().getActiveInstance();
        if (instance != this) {
            instance->pause();
        } else {
            return; // If I'm in control it's over
        }
    }
    // recreate
    mManager = new GUIManager(this);
    // restore
    for (LayoutPrefixMap::iterator it = mLayoutPrefix.begin();
         it != mLayoutPrefix.end();
         ++it) {
        mManager->loadLayout(it->first, it->second);
    }
}

void GuiInstance::pause() {
    delete mManager;
    mManager = 0;
}

template <typename T>
T *GuiInstance::findWidget(const String &name, const String &prefix) {
    if (!mManager) return;
    return mManager->findWidget<T>(name, prefix);
}

template <typename T>
T *GuiInstance::findWidget(const String &name) {
    if (!mManager) return;
    return mManager->findWidget<T>(name);
}


void GuiInstance::loadLayout(const String &layoutFile, const String &prefix) {
    if (!mManager) return;
    mManager->loadLayout(layoutFile, prefix);
    mLayoutPrefix[layoutFile] = prefix;
}

void GuiInstance::unloadLayout(const String &layoutFile) {
    if (!mManager) return;
    mManager->unloadLayout(layoutFile);
    mLayoutPrefix.erase(layoutFile);
}

void GuiInstance::setPointerVisible(bool visible) {
    if (!mManager) return;
    mManager->setPointerVisible(visible);
}

bool GuiInstance::getPointerVisible() {
    if (!mManager) return false;
    return mManager->getPointerVisible();
}


