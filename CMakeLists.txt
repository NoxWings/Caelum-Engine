project(Project-Caelum)
cmake_minimum_required(VERSION 2.6)

# CMAKE MODULES
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake) #cmake find modules location

# REQUIRED LIBRARIES
#message ( STATUS "*CHECKING PKG-CONFIG...")
#find_package(PkgConfig REQUIRED) #Enable PKG-CONFIG suport

message ( STATUS "*CHECKING REQUIRED LIBRARIES...")
set(OGRE_LIBRARIES OgreMain OgrePaging OgreTerrain OgreRTShaderSystem cg)
set(OGRE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/OGRE)
#set(PG_LIBRARIES PagedGeometry)
#set(PG_INCLUDE_DIRS )
set(SKYX_LIBRARIES SkyX)
set(SKYX_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/SKYX)
#set(MYGUI_LIBRARIES )
#set(MYGUI_INCLUDE_DIRS )
set(OIS_LIBRARIES OIS)
set(OIS_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/OIS)
#set(PSMOVE_LIBRARIES)
set(BULLET_LIBRARIES BulletSoftBody BulletDynamics BulletCollision LinearMath)
set(BULLET_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/bullet)
#set(ENET_LIBRARIES)
#set(OPENAL_LIBRARIES)
#set(VORBIS_LIBRARIES)


# LIBRARY INCLUDES & LINK DIRECTORIES
include_directories (
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/external/include
  ${OGRE_INCLUDE_DIRS}
  ${SKYX_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
  ${BULLET_INCLUDE_DIRS}
  )

link_directories (
  ${PROJECT_SOURCE_DIR}/external/bin/
  ${PROJECT_SOURCE_DIR}/external/lib/
  )

set(CMAKE_CXX_FLAGS "-g -Wall")

add_subdirectory(src)

# COMPILATION FLAGS
#message( STATUS "ogre-LD: ${BULLET_LDFLAGS}" ) # LIBS  | pkg-config --libs
#message( STATUS "ogre-CF: ${BULLET_CFLAGS}" ) # CFLAGS | pkg-config --cflags

#message( STATUS "ogre-ID: ${OGRE-Terrain_INCLUDE_DIRS}" ) # CFLAGS (w/o -I)
#message( STATUS "ogre-L: ${OGRE-Terrain_LIBRARIES}" ) # LIBS (w/o -l)
