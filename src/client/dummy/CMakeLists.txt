set(dummy_SRC
  dummy.cpp
)

add_executable(mc_udp_dummy_client ${dummy_SRC})
target_link_libraries(mc_udp_dummy_client mc_udp_client)
target_include_directories(mc_udp_dummy_client PRIVATE "${PROJECT_SOURCE_DIR}/include")
set_target_properties(mc_udp_dummy_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
