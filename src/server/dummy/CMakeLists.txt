set(dummy_SRC
  dummy.cpp
  ${server_SRC}
  ${server_HDR}
)

add_executable(mc_udp_dummy_server ${dummy_SRC})
target_include_directories(mc_udp_dummy_server PRIVATE "${PROJECT_SOURCE_DIR}/include")
set_target_properties(mc_udp_dummy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
