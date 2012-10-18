/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of NAGE.
 */
/* Original version Copyright (C) Scott Bilas, 2000.
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Bilas, 2000"
 */

/* ---------------  EXPLANATION  ----------------------------------

Singleton Pattern Class from Scott Bilas Game Programming Gems.
Ogre3D provides its own OgreSingleton template class but this class has been
implemented to avoid a C++ standart restriction:

"Due to C++ specification (C++ Standard 14.7.3/2) you cannot specialize
a template (like Ogre::Singleton<T>) in other namespace than its declaration's one."

Furthermore this class implements a template initialization of the static
member variable.

----------------------------------------------------------------------*/

#ifndef SRC_PATTERNS_SINGLETON_H_
#define SRC_PATTERNS_SINGLETON_H_

// #define NDEBUG
#include <NagePrerequisites.h>

/** Template class for creating single-instance global classes.
*/
template <typename T> class Singleton {
  protected:
    static T* ms_Singleton;

  public:
    Singleton(void) {
        assert(!ms_Singleton);
        #if defined( _MSC_VER ) && _MSC_VER < 1200
            int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
            msSingleton = (T*)((int)this + offset);
        #else
            ms_Singleton = static_cast<T*>(this);
        #endif
     }

    ~Singleton(void) {
        assert(ms_Singleton);
        ms_Singleton = 0;
    }

    static T& getSingleton(void) {
        assert(ms_Singleton);
        return (*ms_Singleton);
    }

    static T* getSingletonPtr(void) {
        return ms_Singleton;
    }

  private:
    /** Explicit private constructor and operator=.
     *  These are forbidden operations */
    DISALLOW_COPY_AND_ASSIGN(Singleton<T>);
};

template <typename T> T* Singleton<T>::ms_Singleton = 0;

#endif  // SRC_PATTERNS_SINGLETON_H_
