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

# Tests

set( Map_SRCS map_test.cpp )
add_test_exec( Map "${Map_SRCS}" tmx )

set( EventManager_SRCS eventmanager_test.cpp ../src/events/EventMgr.cpp )
add_test_exec( EventManager "${EventManager_SRCS}" "" )

add_test_exec( Logger logger_test.cpp "" )
