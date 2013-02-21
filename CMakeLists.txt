cmake_minimum_required( VERSION 2.8 )
project( marian )

add_definitions( "-Wall -Wextra -std=c++0x" )

find_package( SDL REQUIRED )
find_package( SDL_image REQUIRED )
find_package( OpenGL REQUIRED )
find_package( Boost COMPONENTS system filesystem unit_test_framework REQUIRED )

set( CMAKE_DEBUG_POSTFIX "_d" )

file( COPY ${CMAKE_SOURCE_DIR}/media DESTINATION ${CMAKE_BINARY_DIR}/ )

set( marian_SRCS
  src/Engine.cpp
  src/FpsCounter.cpp
  src/Game.cpp
  src/GameObject.cpp
  src/Hero.cpp
  src/Map.cpp
  src/ResourceMgr.cpp
  src/Sprite.cpp
  src/Texture.cpp
  src/Util.cpp
  src/main.cpp
)

include_directories(
  src
  tmx
  ${SDL_INCLUDE_DIR}
)

add_subdirectory( tmx )

add_executable( ${PROJECT_NAME}
  ${marian_SRCS}
)

target_link_libraries( ${PROJECT_NAME}
  tmx ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${OPENGL_LIBRARIES}
)

enable_testing()
add_subdirectory( test )

