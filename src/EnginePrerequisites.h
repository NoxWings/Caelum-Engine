/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_ENGINEPREREQUISITES_H_
#define SRC_ENGINEPREREQUISITES_H_

//--------COMMON-DEFINES---------------------------

// #define RELEASE_MODE


//-------------------------------------------------
//--------COMMON-INCLUDES--------------------------

#include <assert.h>
#include <string>

//-------------------------------------------------
//--------COMMON-TYPEDEF--------------------------

namespace Caelum {
#ifndef CAELUM_USE_DOUBLE
  typedef float Real;
#else
  #define BT_USE_DOUBLE_PRECISION
  typedef double Real;
#endif
  typedef std::string String;

// Integer formats of fixed bit width
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;
}

//-------------------------------------------------
//--------LIB-DEFINES------------------------------

/// LIBRARY EXPORT DEFINITION

#if defined(ENGINE_STATIC_LIB)     // Project Caelum Engine STATIC LIBRARY (COMPILE & LINK)
    #define _ADDExport
#elif defined(ENGINE_DYNAMIC_LIB)  // Project Caelum Engine DYNAMIC LIBRARY (COMPILE)
    #define _ADDExport __declspec(dllexport)
#else                              // Project Caelum Engine DYNAMIC LIBRARY (LINK)
    #define _ADDExport __declspec(dllimport)
#endif

/// COMPILATION MODE
#if defined(RELEASE_MODE)
        // Define release mode for asserts
    #define NDEGUB
#else
    #include <stdio.h>
#   endif

/// ENDIAN DEFINES
#define CAELUM_ENDIAN_LITTLE 1
#define CAELUM_ENDIAN_BIG 2
#define CAELUM_ENDIAN CAELUM_ENDIAN_BIG

//-------------------------------------------------
//--------MACROS-----------------------------------

/// USEFUL MACROS

#define BIT(x) (1<<(x))

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)

//-------------------------------------------------

#endif  // SRC_ENGINEPREREQUISITES_H_
