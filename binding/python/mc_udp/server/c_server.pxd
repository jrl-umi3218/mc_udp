#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

from libcpp.string cimport string
from libcpp cimport bool as cppbool

cimport mc_udp.data.c_data as c_data

cdef extern from "<mc_udp/server/Server.h>" namespace "mc_udp":
    cdef cppclass Server:
        Server()
        Server(int)
        void restart(int)
        cppbool recv()
        void send()
        void stop()
        const c_data.MultiRobotControl & control()
        c_data.MultiRobotSensors & sensors()
