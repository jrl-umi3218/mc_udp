#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

from libcpp.string cimport string
from libcpp.unordered_map cimport unordered_map
from libcpp.vector cimport vector

cimport c_data

cdef extern from "helper.hpp":
    cdef c_data.RobotSensors & getSensors(unordered_map[string, c_data.RobotSensors] *, const string &)
    cdef c_data.RobotControl & getControl(unordered_map[string, c_data.RobotControl] *, const string &)
    cdef vector[string] getKeys[U](unordered_map[string, U] *)
