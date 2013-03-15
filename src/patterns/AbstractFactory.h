/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_PATTERNS_ABSTRACTFACTORY_H_
#define SRC_PATTERNS_ABSTRACTFACTORY_H_

#include "EnginePrerequisites.h"


/** Template Abstract Factory class
 */
template<typename T> class AbstractFactory {
  public:
    virtual ~AbstractFactory() {}
    virtual T* createInstance(const std::string& name) = 0;
    virtual void destroyInstance(T* instance) = 0;
};

#endif  // SRC_PATTERNS_ABSTRACTFACTORY_H_
