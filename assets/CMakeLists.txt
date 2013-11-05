set ( assets_FILES
  map1.tmx
  map2.tmx
  minecraft_tiles_big.png
  TileKit.png
  TileKit-border.png
  MegaMan_001.png
  cegui
)

file( COPY ${assets_FILES}
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

# To list files in QtCreator
add_custom_target(assets SOURCES ${assets_FILES})
