project(Project-Caelum)
cmake_minimum_required(VERSION 2.6)

# CMAKE MODULES
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake) #cmake find modules location

# REQUIRED LIBRARIES
message ( STATUS "*CHECKING PKG-CONFIG...")
find_package(PkgConfig REQUIRED) #Enable PKG-CONFIG suport

message ( STATUS "*CHECKING REQUIRED LIBRARIES...")
PKG_CHECK_MODULES(OGRE    OGRE-Terrain>=1.7)
#set(PG_LIBRARIES PagedGeometry)
#set(SKYX_LIBRARIES SkyX_01)
PKG_CHECK_MODULES(OIS     OIS)
PKG_CHECK_MODULES(PSMOVE  psmoveapi)
#PKG_CHECK_MODULES(MYGUI   MYGUI)
#PKG_CHECK_MODULES(ENET    libenet)
#PKG_CHECK_MODULES(BULLET  bullet>=2.7)
#PKG_CHECK_MODULES(OPENAL  openal>=1.0)
#PKG_CHECK_MODULES(VORBIS  vorbisfile)
#PKG_CHECK_MODULES(LUA     lua5.1)
#PKG_CHECK_MODULES(LUABIND luabind)

# LIBRARY INCLUDES & LINK DIRECTORIES
include_directories (
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/include
  ${OGRE_INCLUDE_DIRS}
  #${PROJECT_SOURCE_DIR}/include/PagedGeometry
  #${PROJECT_SOURCE_DIR}/include/SkyX
  ${OIS_INCLUDE_DIRS}
  ${PSMOVE_INCLUDE_DIRS}
  #${MYGUI_INCLUDE_DIRS}
  #${ENET_INCLUDE_DIRS}
  #${BULLET_INCLUDE_DIRS}
  #${OPENAL_INCLUDE_DIRS}
  #${VORBIS_INCLUDE_DIRS}
  #${LUA_INCLUDE_DIRS}
  #${LUABIND_INCLUDE_DIRS}
  )

link_directories (
  ${PROJECT_SOURCE_DIR}/external/lib/
  )

set(CMAKE_CXX_FLAGS "-g -Wall")

add_subdirectory(src)

# COMPILATION FLAGS
#message( STATUS "ogre-LD: ${BULLET_LDFLAGS}" ) # LIBS  | pkg-config --libs
#message( STATUS "ogre-CF: ${BULLET_CFLAGS}" ) # CFLAGS | pkg-config --cflags

#message( STATUS "ogre-ID: ${OGRE-Terrain_INCLUDE_DIRS}" ) # CFLAGS (w/o -I)
#message( STATUS "ogre-L: ${OGRE-Terrain_LIBRARIES}" ) # LIBS (w/o -l)
