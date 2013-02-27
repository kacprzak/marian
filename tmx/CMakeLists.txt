add_library(tmx
  TmxMap.cpp
  base64/base64.cpp
)

target_link_libraries( tmx
  ${ZLIB_LIBRARIES}
)
