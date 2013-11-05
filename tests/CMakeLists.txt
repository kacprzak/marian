set( My_Boost_TEST_LIBS
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_executable( map_test map_test.cpp )
target_link_libraries( map_test tmx ${My_Boost_TEST_LIBS} )

add_test(Map map_test)


add_executable( eventmanager_test eventmanager_test.cpp ../src/events/EventMgr.cpp )
target_link_libraries( eventmanager_test ${My_Boost_TEST_LIBS} )

add_test(EventManager eventmanager_test)


add_executable( logger_test logger_test.cpp )
target_link_libraries( logger_test ${My_Boost_TEST_LIBS} )

add_test(Logger logger_test)
