set (
  SRC
  main.cpp
  EnginePrerequisites.h

  core/gameengine.cpp
  core/gameengine.h
  core/gamestate.cpp
  core/gamestate.h
  core/resourcemanager.cpp
  core/resourcemanager.h

  core/log.h
  core/logmanager.cpp
  core/logmanager.h
  core/ogre/ogrelog.cpp
  core/ogre/ogrelog.h
  core/ogre/ogrelogmanager.cpp
  core/ogre/ogrelogmanager.h

  core/pluginmanager.cpp
  core/pluginmanager.h
  core/pluginloader.h

  render/rendermanager.cpp
  render/rendermanager.h
  render/renderlistener.h

  patterns/AbstractFactory.h
  patterns/Singleton.h

  )

add_executable(Caelum ${SRC})
set_target_properties(Caelum PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(
  Caelum
  ${OGRE_LIBRARIES}
  #${PG_LIBRARIES}
  #${SKYX_LIBRARIES}
  ${OIS_LIBRARIES}
  ${PSMOVE_LIBRARIES}
  #${MYGUI_LIBRARIES}
  #${ENET_LIBRARIES}
  #${BULLET_LIBRARIES}
  #${OPENAL_LIBRARIES}
  #${VORBIS_LIBRARIES}
  #${LUA_LIBRARIES}
  #${LUABIND_LIBRARIES}
  )
