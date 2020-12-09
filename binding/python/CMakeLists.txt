#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

add_cython_bindings(mc_udp
  TARGETS mc_udp::data mc_udp::server mc_udp::client
  VERSION 1.0.0
  MODULES mc_udp.data.data
          mc_udp.client.client
          mc_udp.server.server
  EXPORT_SOURCES
    mc_udp/__init__.py
    mc_udp/data/__init__.py
    mc_udp/data/c_data.pxd
    mc_udp/data/data.pxd
    mc_udp/client/__init__.py
    mc_udp/client/c_client.pxd
    mc_udp/client/client.pxd
    mc_udp/server/__init__.py
    mc_udp/server/c_server.pxd
    mc_udp/server/server.pxd
  PRIVATE_SOURCES
    mc_udp/data/data.pyx
    mc_udp/data/c_data_private.pxd
    mc_udp/client/client.pyx
    mc_udp/server/server.pyx
    include/helper.hpp
)
