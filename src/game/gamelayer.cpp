#include "game/gamelayer.h"

using namespace Caelum;

GameLayer::~GameLayer() {
    destroyAllComponents();
}

Component *GameLayer::getComponentByName(const String &name) {
    return mComponents.findItem(name);
}

void GameLayer::destroyComponent(const String& name) {
    delete mComponents.findItem(name);
    mComponents.removeItem(name);
}

void GameLayer::destroyComponent(Component *component) {
    if (mComponents.removeItem(component)) {
        delete component;
    }
}

void GameLayer::destroyAllComponents() {
    while (!mComponents.empty()) {
        Component *comp = mComponents.getFirstItem();
        mComponents.removeItem(comp);
        delete comp;
    }
}

void GameLayer::addComponent(Component *component) {
    mComponents.addItem(component->getName(), component);
}
