set(server_HDR
  "${include_DIR}/server/Server.h"
  ${data_HDR}
)

set(server_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp"
  ${data_SRC}
)

add_subdirectory(dummy)
add_subdirectory(openrtm)
