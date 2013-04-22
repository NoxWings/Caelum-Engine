#include "layermanager.h"

using namespace Caelum;


LayerManager::~LayerManager() {
    GameLayer *layer;
    while (!mLayers.empty()) {
        layer = mLayers.getFirstItem();
        mLayers.removeItem(layer);
        delete layer;
    }
}

void LayerManager::destroyLayer(const String &name) {
    delete mLayers.findItem(name);
    mLayers.removeItem(name);
}

void LayerManager::destroyLayer(GameLayer *layer) {
    // Remove item returns true if the layer was actually part of this collection
    // and therefore removed
    if (mLayers.removeItem(layer)) {
        delete layer;
    }
}

void LayerManager::addLayer(GameLayer *layer) {
    mLayers.addItem(layer->getName(), layer);
}
