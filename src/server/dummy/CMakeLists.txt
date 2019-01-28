set(dummy_SRC
  dummy.cpp
  ${server_SRC}
  ${server_HDR}
)

add_executable(mc_nng_dummy_server ${dummy_SRC})
target_include_directories(mc_nng_dummy_server PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(mc_nng_dummy_server nng)
set_target_properties(mc_nng_dummy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
