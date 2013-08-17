set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set( marian_HDRS
  actors/Box.h
  actors/Hero.h
  actors/Ground.h

  Animation.h
  AiComponent.h
  Console.h
  Engine.h
  EventType.h
  Event.h
  EventManager.h
  FpsCounter.h
  Game.h
  Actor.h
  ActorComponent.h
  ActorComponentId.h
  ActorCategory.h
  ActorFactory.h
  ActorState.h
  PhysicsEngine.h
  Box2dPhysicsEngine.h
  PhysicsComponent.h
  ControlComponent.h
  RenderComponent.h
  ImageRenderComponent.h
  GuiMgr.h
  Image.h
  Map.h
  Playable.h
  ResourceMgr.h
  ScriptMgr.h
#  Sensor.h
  Singleton.h
  StateMachine.h
  Texture.h
  Util.h

  debugdraw/Render.h
)

set( marian_SRCS
  actors/Box.cpp
  actors/Ground.cpp
  actors/Hero.cpp

  Box2dPhysicsEngine.cpp
  Console.cpp
  Engine.cpp
  EventManager.cpp
  FpsCounter.cpp
  Game.cpp
  GuiMgr.cpp
  Map.cpp
  ResourceMgr.cpp
#  Sensor.cpp
  ScriptMgr.cpp
  Texture.cpp
  Util.cpp
  main.cpp

  debugdraw/Render.cpp
)

add_executable( marian
  ${marian_HDRS} ${marian_SRCS}
)

set_target_properties( marian PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries( marian
  tmx
  ${BOX2D_LIBRARY}
  ${SDL_LIBRARY}
  ${SDLIMAGE_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${LUA_LIBRARIES}
#  ${Boost_LIBRARIES}
#  ${GLUT_LIBRARY}
  ${CEGUI_LIBRARY}
)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/marian_d
    DEPENDS marian
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
