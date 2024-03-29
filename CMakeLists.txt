cmake_minimum_required( VERSION 3.12 )
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0115 NEW)
cmake_policy(SET CMP0117 NEW)

project( marian )

# The version number.
set( MARIAN_VERSION_MAJOR 0 )
set( MARIAN_VERSION_MINOR 2 )

if( MSVC )
  add_definitions( "/W2" )
else()
  add_definitions( "-Wall -Wextra -Wno-unused-parameter -std=c++1y" )
endif()

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" )

set( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set( CMAKE_DEBUG_POSTFIX "_d" )

option(USE_MYGUI OFF)

find_package( SDL2 REQUIRED )
find_package( SDL2_image REQUIRED )
find_package( opengl_system REQUIRED )
find_package( Boost REQUIRED )
if( UNIX )
  find_package( Boost COMPONENTS unit_test_framework )
endif()
find_package( Box2D REQUIRED )
find_package( Lua REQUIRED )
find_package( rapidxml REQUIRED )

if(USE_MYGUI)
  find_package( MyGUI REQUIRED )
  include_directories( ${MYGUI_INCLUDE_DIRS} )
  link_directories( ${MYGUI_LIB_DIR} )
  set( MYGUI_LIBRARIES ${MYGUI_LIBRARIES} MyGUI.OpenGLPlatform )
  set( EXTRA_LIBS ${EXTRA_LIBS} ${MYGUI_LIBRARIES} )
endif()

configure_file(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
  )

configure_file(
  ${PROJECT_SOURCE_DIR}/startup.lua.in
  ${PROJECT_BINARY_DIR}/startup.lua
  )

#find_package( rapid-tmx REQUIRED )
add_subdirectory( rapid-tmx )

add_subdirectory( src )
#add_subdirectory( assets )
#add_subdirectory( scripts )
#add_subdirectory( tools )

if( Boost_UNIT_TEST_FRAMEWORK_FOUND )
  enable_testing()
  add_subdirectory( tests )
endif()

# add a target to generate API documentation with Doxygen
find_package( Doxygen )
if( DOXYGEN_FOUND )
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY )
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif( DOXYGEN_FOUND )

# For clangd support
if(UNIX)
  execute_process(COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
