/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef LISTENERREGISTRANT_H
#define LISTENERREGISTRANT_H

#include "EnginePrerequisites.h"

#include <list>

template <typename T> class SimpleCollection {
  public:
    SimpleCollection() {}
    ~SimpleCollection() { mItems.clear();}

    void addItem(T listener) {
        mItems.push_back(listener);
    }

    void removeItem(T listener) {
        mItems.remove(listener);
    }


  protected:
    typedef std::list<T> ItemList;
    ItemList mItems;
};


#endif // LISTENERREGISTRANT_H
