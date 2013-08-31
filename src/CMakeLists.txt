set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set( marian_HDRS
  actors/Box.h
  actors/Hero.h
  actors/Ground.h
  actors/Sensor.h

  components/AiComponent.h
  components/ActorComponent.h
  components/ActorComponentId.h
  components/PhysicsComponent.h
  components/ControlComponent.h
  components/RenderComponent.h

  graphics/Animation.h
  graphics/Image.h
  graphics/Texture.h
  graphics/SpriteNode.h
  graphics/HeroNode.h
  graphics/MapNode.h

  Console.h
  Engine.h
  EventType.h
  Event.h
  EventMgr.h
  FpsCounter.h
  Game.h
  GameLogic.h
  BaseGameLogic.h
  GameView.h
  HumanView.h
  Actor.h
  ActorCategory.h
  ActorFactory.h
  PhysicsEngine.h
  Box2dPhysicsEngine.h
  GuiMgr.h
  Map.h
#  Playable.h
  ResourceMgr.h
  ScriptMgr.h
  Singleton.h
  StateMachine.h
  Util.h

  debugdraw/Render.h
)

set( marian_SRCS
  actors/Box.cpp
  actors/Ground.cpp
  actors/Hero.cpp
  actors/Sensor.cpp

  graphics/Texture.cpp
  graphics/SpriteNode.cpp
  graphics/HeroNode.cpp
  graphics/MapNode.cpp

  Box2dPhysicsEngine.cpp
  Console.cpp
  Engine.cpp
  EventMgr.cpp
  FpsCounter.cpp
  Game.cpp
  HumanView.cpp
  BaseGameLogic.cpp
  GuiMgr.cpp
  Map.cpp
  ResourceMgr.cpp
  ScriptMgr.cpp
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
  ${SDL2_LIBRARY}
#  ${SDLIMAGE_LIBRARY}
  ${SDL2IMAGE_LIBRARY}
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
