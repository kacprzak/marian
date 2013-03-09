set ( script_FILES
  startup.lua
)

file( COPY ${script_FILES}
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
