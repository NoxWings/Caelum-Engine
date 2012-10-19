/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of NAGE.
 */

#ifndef SRC_PATTERNS_ABSTRACTFACTORY_H_
#define SRC_PATTERNS_ABSTRACTFACTORY_H_

#include "NagePrerequisites.h"

template<typename T> class AbstractFactory {
  public:
    virtual ~AbstractFactory() {}
    virtual const std::string& getType() = 0;
    virtual T* createInstance(const std::string& name) = 0;
    virtual void destroyInstance(T* instance) = 0;

  private:
};

#endif  // SRC_PATTERNS_ABSTRACTFACTORY_H_
