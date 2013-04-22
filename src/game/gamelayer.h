/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_GAME_GAMELAYER_H
#define SRC_GAME_GAMELAYER_H

#include "EnginePrerequisites.h"
#include "game/component.h"
#include "patterns/AssociativeCollection.h"

namespace Caelum {

class GameLayer {
  public:
    GameLayer(const String& name, const String& typeName) {mName = name; mTypeName = typeName;}
    virtual ~GameLayer();

    const String& getName() {return mName;}
    const String& getTypeName() {return mTypeName;}

    //virtual Component* createComponentByTypeName(const String& name, const String& typeName) = 0;
    void destroyComponent(const String& name);
    void destroyComponent(Component *component);

    virtual void update(Real deltaTime) = 0;

  protected:
    void addComponent(Component* component);

    String mName;
    String mTypeName;
    AssociativeCollection<String, Component*> mComponents;
};

}

#endif // SRC_GAME_GAMELAYER_H
