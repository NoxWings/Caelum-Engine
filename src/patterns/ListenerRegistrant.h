#ifndef LISTENERREGISTRANT_H
#define LISTENERREGISTRANT_H

#include "EnginePrerequisites.h"

#include <list>

template <typename T> class ListenerRegistrant {
  public:
    ListenerRegistrant() {}
    ~ListenerRegistrant() { mListeners.clear();}

    void addListener(T* listener) {
        mListeners.push_back(listener);
    }

    void removeListener(T* listener) {
        mListeners.remove(listener);
    }


  protected:
    typedef std::list<T*> ListenerList;
    ListenerList mListeners;
};


#endif // LISTENERREGISTRANT_H
