set(server_HDR
  "${include_DIR}/server/Server.h"
)

set(server_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp"
)

add_library(mc_udp_server SHARED ${server_SRC} ${server_HDR})
add_library(mc_udp::server ALIAS mc_udp_server)
set_target_properties(mc_udp_server PROPERTIES EXPORT_NAME server)
target_link_libraries(mc_udp_server PUBLIC mc_udp_data)
target_include_directories(mc_udp_server PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
set_target_properties(mc_udp_server PROPERTIES PUBLIC_HEADER "${server_HDR}")

install(
  TARGETS mc_udp_server
  EXPORT mc_udp_serverTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include/mc_udp/server
)

install(EXPORT mc_udp_serverTargets
  FILE mc_udp_serverTargets.cmake
  NAMESPACE mc_udp::
  DESTINATION lib/cmake/mc_udp
)

if(${BUILD_DUMMY_SERVER})
  add_subdirectory(dummy)
endif()

if(${BUILD_OPENRTM_SERVER})
  add_subdirectory(openrtm)
endif()
