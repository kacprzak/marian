set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set( marian_HDRS
  Box.h
  Engine.h
  FpsCounter.h
  Game.h
  Ground.h
  Hero.h
  Image.h
  Map.h
  ResourceMgr.h
  Texture.h
  Util.h
  Vect.h
  debugdraw/Render.h
)

set( marian_SRCS
  Box.cpp
  Engine.cpp
  FpsCounter.cpp
  Game.cpp
  Ground.cpp
  Hero.cpp
  Map.cpp
  ResourceMgr.cpp
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
  tmx ${BOX2D_LIBRARY} ${SDL_LIBRARY}
  ${SDLIMAGE_LIBRARY} ${OPENGL_LIBRARIES}
#  ${Boost_LIBRARIES}
#  ${GLUT_LIBRARY}
)
