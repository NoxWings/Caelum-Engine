#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "EnginePrerequisites.h"

#include "game/gamelayer.h"
#include "patterns/AssociativeCollection.h"

namespace Caelum {

class LayerManager {
  public:
    LayerManager() {}
    virtual ~LayerManager();

    virtual GameLayer* createLayer(const String& name, const String& typeName) = 0;
    void destroyLayer(const String& name);
    void destroyLayer(GameLayer *layer);

  protected:
    void addLayer(GameLayer* layer);

    AssociativeCollection<String, GameLayer*> mLayers;
};

}

#endif // LAYERMANAGER_H
