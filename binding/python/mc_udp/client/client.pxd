#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport c_client

cdef class Client(object):
    cdef c_client.Client * impl
