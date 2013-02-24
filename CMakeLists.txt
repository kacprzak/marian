cmake_minimum_required( VERSION 2.8 )
project( marian )

add_definitions( "-Wall -Wextra -std=c++0x" )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package( SDL REQUIRED )
find_package( SDL_image REQUIRED )
find_package( OpenGL REQUIRED )
find_package( Boost COMPONENTS iostreams unit_test_framework REQUIRED )
find_package( Box2D REQUIRED )
#find_package( GLUT REQUIRED )

set( CMAKE_DEBUG_POSTFIX "_d" )

file( COPY ${CMAKE_SOURCE_DIR}/media DESTINATION ${CMAKE_BINARY_DIR}/ )

include_directories(
  src
  tmx
  ${SDL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
#  ${GLUT_INCLUDE_DIRS}
)

add_subdirectory( tmx )
add_subdirectory( src )

enable_testing()
add_subdirectory( test )

