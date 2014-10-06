/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_PATTERNS_ASSOCIATIVECOLLECTION_H
#define SRC_PATTERNS_ASSOCIATIVECOLLECTION_H

#include "EnginePrerequisites.h"

#include <map>

template <typename S, typename T> class AssociativeCollection {
  public:
    AssociativeCollection() {}
    ~AssociativeCollection() { mItems.clear();}

    void addItem(const S& key, T item) {
        mItems.insert(std::pair<S,T>(key, item));
    }

    void removeItem(const S& key) {
        mItems.erase(key);
    }

    bool removeItem(T item) {
        bool found = false;
        for (typename ItemMap::iterator it = mItems.begin();
             (it != mItems.end()) && (!found);
             ++it) {
            T myItem = it->second;
            if (myItem == item) {
                mItems.erase(it);
                found = true;
            }
        }
        return found;
    }

    T findItem(S key) {
        typename ItemMap::iterator it = mItems.find(key);
        if (it != mItems.end()) {
            return (it->second);
        }
        return 0;
    }

    T getFirstItem() {
        if ( !(mItems.empty()) ) {
            typename ItemMap::iterator it = mItems.begin();
            return it->second;
        }
        return 0;
    }

    bool empty() {
        return mItems.empty();
    }

    typedef std::map<S,T> ItemMap;
    ItemMap mItems;
};

#endif // SRC_PATTERNS_ASSOCIATIVECOLLECTION_H
