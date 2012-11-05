set (
  SRC
  main.cpp
  EnginePrerequisites.h
  input/inputmanager.cpp
  input/inputmanager.h
  input/keylistener.cpp
  input/keylistener.h
  input/psmovecontroller.cpp
  input/psmovecontroller.h
  input/psmovecontrollerfactory.cpp
  input/psmovecontrollerfactory.h
  input/mouselistener.cpp
  input/mouselistener.h
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
