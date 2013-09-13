set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set( marian_HDRS
  actors/Actor.h
  actors/ActorCategory.h
  actors/ActorFactory.h
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

  input/KeyboardHandler.h
  input/HeroController.h

  network/Packet.h
  network/NetSocket.h
  network/ListenNetSocket.h
  network/BaseSocketMgr.h
  network/ClientSocketMgr.h
  network/RemoteEventSocket.h
  network/GameServerListenNetSocket.h
  network/NetworkEventForwarder.h
  network/RemoteGameView.h
  network/RemoteGameLogic.cpp

  events/EventType.h
  events/Event.h
  events/EventMgr.h

  gui/Console.h
  gui/GuiMgr.h

  Logger.h
  Engine.h
  FpsCounter.h
  Game.h
  GameLogic.h
  BaseGameLogic.h
  GameView.h
  HumanView.h
  PhysicsEngine.h
  Box2dPhysicsEngine.h
  Map.h
  ResourceMgr.h
  ScriptMgr.h
  Singleton.h
  StateMachine.h
  Util.h

  debugdraw/Render.h
)

set( marian_SRCS
  actors/Actor.cpp
  actors/Box.cpp
  actors/Ground.cpp
  actors/Hero.cpp
  actors/Sensor.cpp

  graphics/Texture.cpp
  graphics/SpriteNode.cpp
  graphics/HeroNode.cpp
  graphics/MapNode.cpp

  input/HeroController.cpp

  network/Packet.cpp
  network/NetSocket.cpp
  network/ListenNetSocket.cpp
  network/BaseSocketMgr.cpp
  network/ClientSocketMgr.cpp
  network/RemoteEventSocket.cpp
  network/GameServerListenNetSocket.cpp
  network/NetworkEventForwarder.cpp
  network/RemoteGameView.cpp
  network/RemoteGameLogic.cpp

  events/EventMgr.cpp

  gui/Console.cpp
  gui/GuiMgr.cpp

  Logger.cpp
  Box2dPhysicsEngine.cpp
  Engine.cpp
  FpsCounter.cpp
  Game.cpp
  HumanView.cpp
  BaseGameLogic.cpp
  Map.cpp
  ResourceMgr.cpp
  ScriptMgr.cpp
  Util.cpp

  debugdraw/Render.cpp
)

set( LINK_LIBS
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

add_executable( marian main.cpp ${marian_HDRS} ${marian_SRCS} )
set_target_properties( marian PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries( marian ${LINK_LIBS} )

add_executable( mariansrv server.cpp ${marian_HDRS} ${marian_SRCS} )
set_target_properties( mariansrv PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries( mariansrv ${LINK_LIBS} )

add_executable( mariancli client.cpp ${marian_HDRS} ${marian_SRCS} )
set_target_properties( mariancli PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries( mariancli ${LINK_LIBS} )

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/marian_d
    DEPENDS marian
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
