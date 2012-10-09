/*
 *  NAGE (Not Another Game Engine)
 *  (name of the project could be changed in future revisions)
 *
 *  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  This file is part of NAGE.
 */

#ifndef LIBPREREQUISITES_HPP
#define LIBPREREQUISITES_HPP

//---------------------

//#define RELEASE_MODE

//---------------------

/// LIBRARY EXPORT DEFINITION
#	if defined(NAGE_STATIC_LIB)
        // Import definition
#		define _NAGEExport __declspec( dllimport )
#	else
        // Export definition
#		define _NAGEExport __declspec( dllexport )
#	endif

/// COMPILATION MODE
#   if defined(RELEASE_MODE)
        // Define release mode for asserts
#       define NDEGUB
#   endif


/// USEFUL MACROS
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)

#endif // LIBPREREQUISITES_HPP
