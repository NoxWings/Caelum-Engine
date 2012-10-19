set (
  SRC
  main.cpp
  NagePrerequisites.h
  input/inputmanager.cpp
  input/inputmanager.h
  input/keylistener.cpp
  input/keylistener.h
  input/motioncontroller.cpp
  input/motioncontroller.h
  input/motioncontrollerfactory.cpp
  input/motioncontrollerfactory.h
  input/mouselistener.cpp
  input/mouselistener.h
  patterns/AbstractFactory.h
  patterns/Singleton.h

  )

add_executable(NAGE ${SRC})
set_target_properties(NAGE PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(
  NAGE
  ${OGRE_LIBRARIES}
  #${PG_LIBRARIES}
  #${SKYX_LIBRARIES}
  ${OIS_LIBRARIES}
  #${MYGUI_LIBRARIES}
  #${ENET_LIBRARIES}
  #${BULLET_LIBRARIES}
  #${OPENAL_LIBRARIES}
  #${VORBIS_LIBRARIES}
  #${LUA_LIBRARIES}
  #${LUABIND_LIBRARIES}
  )
