project(Project-Caelum)
cmake_minimum_required(VERSION 2.6)

# CMAKE MODULES
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake) #cmake find modules location

# REQUIRED LIBRARIES
#message ( STATUS "*CHECKING PKG-CONFIG...")
#find_package(PkgConfig REQUIRED) #Enable PKG-CONFIG suport

message ( STATUS "*CHECKING REQUIRED LIBRARIES...")
# 1.- OGRE3D
set(OGRE_LIBRARIES OgreMain OgrePaging OgreTerrain OgreRTShaderSystem cg)
set(OGRE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/OGRE)
# 2.- PAGED GEOMETRY
#set(PG_LIBRARIES PagedGeometry)
#set(PG_INCLUDE_DIRS )
# 3.- SKYX
set(SKYX_LIBRARIES SkyX)
set(SKYX_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/SKYX)
# 4.- HYDRAX
set(HYDRAX_LIBRARIES Hydrax)
set(HYDRAX_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/Hydrax)
# 5.- MYGUI
set(MYGUI_LIBRARIES MyGUIEngine MyGUI.OgrePlatform)
set(MYGUI_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/external/include/MyGUI
    ${PROJECT_SOURCE_DIR}/external/include/MyGUIOgrePlatform)
# 6.- OIS
set(OIS_LIBRARIES OIS)
set(OIS_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/OIS)
# 7.- PSMOVE
# 8.- BULLET
set(BULLET_LIBRARIES BulletSoftBody BulletDynamics BulletCollision LinearMath)
set(BULLET_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/bullet)
# 9.- ENET
set(ENET_LIBRARIES enet ws2_32 winmm)
set(ENET_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/enet)
# 10.- OPENAL
set(OPENAL_LIBRARIES OpenAL)
set(OPENAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include/AL)
# 11.- Ogg-VORBIS
set(VORBIS_LIBRARIES vorbisfile vorbis ogg)
set(VORBIS_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/external/include/ogg
    ${PROJECT_SOURCE_DIR}/external/include/vorbis
)


# LIBRARY INCLUDES & LINK DIRECTORIES
include_directories (
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/external/include
  ${OGRE_INCLUDE_DIRS}
  ${SKYX_INCLUDE_DIRS}
  ${HYDRAX_INCLUDE_DIRS}
  ${MYGUI_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
  ${BULLET_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIRS}
  ${VORBIS_INCLUDE_DIRS}
  #${ENET_INCLUDE_DIRS}
  )

link_directories (
  ${PROJECT_SOURCE_DIR}/external/bin/
  ${PROJECT_SOURCE_DIR}/external/lib/
  )

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -flto") # Link time optimization
#set(CXX_COVERAGE_LINK_FLAGS "-optl-static -static-libgcc -static-libstdc++")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_COVERAGE_LINK_FLAGS}")

add_subdirectory(src)

# COMPILATION FLAGS
#message( STATUS "ogre-LD: ${BULLET_LDFLAGS}" ) # LIBS  | pkg-config --libs
#message( STATUS "ogre-CF: ${BULLET_CFLAGS}" ) # CFLAGS | pkg-config --cflags

#message( STATUS "ogre-ID: ${OGRE-Terrain_INCLUDE_DIRS}" ) # CFLAGS (w/o -I)
#message( STATUS "ogre-L: ${OGRE-Terrain_LIBRARIES}" ) # LIBS (w/o -l)
