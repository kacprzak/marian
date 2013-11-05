set ( tools_FILES
  prepend.rb
  scaffold.rb
  tileborder.rb
)

# To list files in QtCreator
add_custom_target(tools SOURCES ${tools_FILES})
