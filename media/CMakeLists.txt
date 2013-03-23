set ( media_FILES
  map1.tmx
  map2.tmx
  minecraft_tiles_big.png
  TileKit.png
  MegaMan_001.png
  cegui
)

file( COPY ${media_FILES}
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

# To list files in QtCreator
add_custom_target(media SOURCES ${media_FILES})
