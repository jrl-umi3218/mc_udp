#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

from libcpp.string cimport string
from libcpp cimport bool as cppbool

cimport mc_udp.data.c_data as c_data

cdef extern from "<mc_udp/client/Client.h>" namespace "mc_udp":
    cdef cppclass Client:
        Client(string, int)
        void init()
        cppbool recv()
        void send()
        c_data.MultiRobotControl & control()
        const c_data.MultiRobotSensors & sensors()
