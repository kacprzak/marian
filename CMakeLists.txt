cmake_minimum_required( VERSION 2.8 )
project( marian )

# The version number.
set( MARIAN_VERSION_MAJOR 0 )
set( MARIAN_VERSION_MINOR 2 )

option( USE_CEGUI "Use CEGUI for game GUI" OFF )
option( USE_MYGUI "Use MyGUI for game GUI" ON )

if( MSVC )
  #
else()
  add_definitions( "-Wall -Wextra -Wno-unused-parameter -std=c++0x" )
endif()

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" )

set( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set( CMAKE_DEBUG_POSTFIX "_d" )

find_package( SDL2 REQUIRED )
find_package( SDL2_image REQUIRED )
find_package( OpenGL REQUIRED )
find_package( Boost REQUIRED )
find_package( Boost COMPONENTS unit_test_framework )
find_package( Box2D REQUIRED )
#find_package( GLUT REQUIRED )
find_package( ZLIB REQUIRED )
find_package( Lua52 REQUIRED )

if( USE_CEGUI )
    find_package( CEGUI REQUIRED COMPONENTS OPENGL )
    include_directories( ${CEGUI_INCLUDE_DIR} )
    set( EXTRA_LIBS ${EXTRA_LIBS} ${CEGUI_LIBRARY} )
endif( USE_CEGUI )

if( USE_MYGUI )
    set( MYGUI_HOME ${PROJECT_SOURCE_DIR}/../mygui )
    set( MYGUI_INCLUDE_DIR ${MYGUI_HOME}/MyGUIEngine/include )
    set( MYGUI_PLATFORM_INCLUDE_DIR ${MYGUI_HOME}/Platforms/OpenGL/OpenGLPlatform/include )
    set( MYGUI_LIBS MyGUIEngine_d MyGUI.OpenGLPlatform_d )

    if( WIN32 )
        set( MYGUI_LINK_DIR ${MYGUI_HOME}/../mygui-build/lib/Debug )
    else()
        set( MYGUI_LINK_DIR ${MYGUI_HOME}/../mygui-debug/lib )
    endif()

    #find_package( MyGui REQUIRED )
    include_directories( ${MYGUI_INCLUDE_DIR} )
    include_directories( ${MYGUI_PLATFORM_INCLUDE_DIR} )
    link_directories( ${MYGUI_LINK_DIR} )
    set( EXTRA_LIBS ${EXTRA_LIBS} ${MYGUI_LIBS} )
endif( USE_MYGUI )
	
configure_file(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
)

configure_file(
  ${PROJECT_SOURCE_DIR}/startup.lua.in
  ${PROJECT_BINARY_DIR}/startup.lua
)

include_directories(
  ${PROJECT_BINARY_DIR}
  src
  tmx
  ${SDL2_INCLUDE_DIR}
  ${SDL2IMAGE_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${BOX2D_INCLUDE_DIR}
#  ${GLUT_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
)

add_subdirectory( tmx )
add_subdirectory( src )
add_subdirectory( assets )
add_subdirectory( scripts )
add_subdirectory( tools )

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
