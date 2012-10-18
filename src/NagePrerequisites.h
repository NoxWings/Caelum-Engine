/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of NAGE.
 */

#ifndef NAGEPREREQUISITES_HPP
#define NAGEREREQUISITES_HPP

//--------COMMON-DEFINES---------------------------

//#define RELEASE_MODE


//-------------------------------------------------
//--------COMMON-INCLUDES--------------------------

#include <assert.h>

//-------------------------------------------------
//--------LIB-DEFINES------------------------------

/// LIBRARY EXPORT DEFINITION
#if defined(NAGE_DYNAMIC_LIB)
    // Export definition
#   define _NAGEExport __declspec( dllexport )
#else
// Import definition
#   define _NAGEExport __declspec( dllimport )
#endif

/// COMPILATION MODE
#   if defined(RELEASE_MODE)
        // Define release mode for asserts
#       define NDEGUB
#   endif

//-------------------------------------------------
//--------MACROS-----------------------------------

/// USEFUL MACROS
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)

//-------------------------------------------------


#endif // NAGEPREREQUISITES_HPP
