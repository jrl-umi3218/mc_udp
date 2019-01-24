set(dummy_SRC
  dummy.cpp
  ${client_SRC}
  ${client_HDR}
)

add_executable(mc_nng_dummy_client ${dummy_SRC})
target_include_directories(mc_nng_dummy_client PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(mc_nng_dummy_client nng)
