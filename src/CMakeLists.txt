set (
  SRC
  main.cpp
  teststate.cpp
  teststate.h

  EnginePrerequisites.h

  core/ogre/ogrelog.cpp
  core/ogre/ogrelog.h
  core/ogre/ogrelogmanager.cpp
  core/ogre/ogrelogmanager.h
  core/configfile.cpp
  core/configfile.h
  core/gameengine.cpp
  core/gameengine.h
  core/log.h
  core/logmanager.cpp
  core/logmanager.h
  core/pluginloader.h
  core/pluginmanager.cpp
  core/pluginmanager.h
  core/preferencemanager.cpp
  core/preferencemanager.h
  core/resourcemanager.cpp
  core/resourcemanager.h

  game/component.h
  game/eventlistener.h
  game/fixedcomponent.cpp
  game/fixedcomponent.h
  game/gamelayer.cpp
  game/gamelayer.h
  game/gamemanager.cpp
  game/gamemanager.h
  game/gameobject.cpp
  game/gameobject.h
  game/gamestate.cpp
  game/gamestate.h
  game/layermanager.cpp
  game/layermanager.h
  game/movablecomponent.cpp
  game/movablecomponent.h
  game/fixedcomponent.h
  game/scene.cpp
  game/scene.h
  game/scenemanager.cpp
  game/scenemanager.h

  math/matrix3.cpp
  math/matrix3.h
  math/quaternion.cpp
  math/quaternion.h
  math/vector3.cpp
  math/vector3.h
  math/unitconversor.h
  math/math.cpp
  math/math.h

  patterns/AbstractFactory.h
  patterns/AssociativeCollection.h
  patterns/SimpleCollection.h
  patterns/Singleton.h

  render/ogre/ogrerendermanager.cpp
  render/ogre/ogrerendermanager.h
  render/ogre/ogrerenderwindow.cpp
  render/ogre/ogrerenderwindow.h
  render/renderlayer.cpp
  render/renderlayer.h
  render/renderlistener.h
  render/rendermanager.cpp
  render/rendermanager.h
  render/renderwindow.cpp
  render/renderwindow.h
  render/windowlistener.h
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
