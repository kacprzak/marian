set ( script_FILES
  startup.lua
  map2_init.lua
)

file( COPY ${script_FILES}
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
