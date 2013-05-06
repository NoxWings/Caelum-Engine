#include "game/gamelayer.h"

using namespace Caelum;

GameLayer::~GameLayer() {
    Component *comp;
    while (!mComponents.empty()) {
        comp = mComponents.getFirstItem();
        mComponents.removeItem(comp);
        delete comp;
    }
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

void GameLayer::addComponent(Component *component) {
    mComponents.addItem(component->getName(), component);
}
