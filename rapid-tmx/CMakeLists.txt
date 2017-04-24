cmake_minimum_required( VERSION 2.8 )
project( rapid-tmx )

find_package( Boost REQUIRED )
if( UNIX )
  find_package( Boost COMPONENTS unit_test_framework )
endif()
find_package( ZLIB REQUIRED )

add_library(tmx
  TmxMap.h
  TmxMap.cpp
  base64/base64.cpp
)

target_link_libraries( tmx
  ${ZLIB_LIBRARIES}
)

# add_test_exec
macro( add_test_exec test_name files libs )
    set( My_Boost_TEST_LIBS
      ${Boost_FILESYSTEM_LIBRARY}
      ${Boost_SYSTEM_LIBRARY}
      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

    string( TOLOWER ${test_name} test_lower_name )

    add_executable( ${test_lower_name}_test ${files} )
    target_link_libraries( ${test_lower_name}_test ${libs} ${My_Boost_TEST_LIBS} )

    add_test( ${test_name} ${test_lower_name}_test )
endmacro( add_test_exec )


if( Boost_UNIT_TEST_FRAMEWORK_FOUND )
  enable_testing()
  set( Map_SRCS map_test.cpp )
  add_test_exec( Map "${Map_SRCS}" tmx )
endif()
