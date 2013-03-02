cmake_minimum_required( VERSION 2.8 )
project( marian )

if( MSVC )
  #
else()
  add_definitions( "-Wall -Wextra -std=c++0x" )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package( SDL REQUIRED )
find_package( SDL_image REQUIRED )
find_package( OpenGL REQUIRED )
find_package( Boost REQUIRED )
find_package( Boost COMPONENTS unit_test_framework )
find_package( Box2D REQUIRED )
#find_package( GLUT REQUIRED )
find_package( ZLIB REQUIRED )

set( CMAKE_DEBUG_POSTFIX "_d" )

include_directories(
  src
  tmx
  ${SDL_INCLUDE_DIR}
  ${SDL_IMAGE_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${BOX2D_INCLUDE_DIR}
#  ${GLUT_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIR}
)

add_subdirectory( tmx )
add_subdirectory( src )
add_subdirectory( media )

if ( Boost_UNIT_TEST_FRAMEWORK_FOUND ) 
  enable_testing()
  add_subdirectory( test )
endif()

