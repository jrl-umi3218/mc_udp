#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport c_server

cdef class Server(object):
    cdef c_server.Server * impl
