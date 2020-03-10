#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

set(dummy_SRC
  dummy.cpp
)

add_executable(mc_udp_dummy_server ${dummy_SRC})
target_link_libraries(mc_udp_dummy_server mc_udp_server)
set_target_properties(mc_udp_dummy_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
