#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport c_data

from libcpp cimport bool as cppbool

from libcpp.string cimport string
from libcpp.unordered_map cimport unordered_map

cdef class ForceSensor(object):
    cdef c_data.ForceSensor * impl
    cdef cppbool is_writable

cdef ForceSensor ForceSensorFromRef(c_data.ForceSensor &)
cdef ForceSensor ForceSensorFromCRef(const c_data.ForceSensor &)

cdef class RobotSensors(object):
    cdef c_data.RobotSensors * impl
    cdef cppbool is_writable

cdef RobotSensors RobotSensorsFromRef(c_data.RobotSensors &)
cdef RobotSensors RobotSensorsFromCRef(const c_data.RobotSensors &)

cdef class RobotControl(object):
    cdef c_data.RobotControl * impl
    cdef cppbool is_writable

cdef RobotControl RobotControlFromRef(c_data.RobotControl &)
cdef RobotControl RobotControlFromCRef(const c_data.RobotControl &)

cdef class _ControlMapWrapper(object):
    cdef unordered_map[string, c_data.RobotControl] * impl
    cdef cppbool is_writable

cdef _ControlMapWrapper _ControlMapFromRef(unordered_map[string, c_data.RobotControl] &)
cdef _ControlMapWrapper _ControlMapFromCRef(const unordered_map[string, c_data.RobotControl] &)

cdef class MultiRobotControl(object):
    cdef c_data.MultiRobotControl * impl
    cdef cppbool is_writable

cdef MultiRobotControl MultiRobotControlFromRef(c_data.MultiRobotControl &)
cdef MultiRobotControl MultiRobotControlFromCRef(const c_data.MultiRobotControl &)

cdef class _SensorsMapWrapper(object):
    cdef unordered_map[string, c_data.RobotSensors] * impl
    cdef cppbool is_writable

cdef _SensorsMapWrapper _SensorsMapFromRef(unordered_map[string, c_data.RobotSensors] &)
cdef _SensorsMapWrapper _SensorsMapFromCRef(const unordered_map[string, c_data.RobotSensors] &)

cdef class MultiRobotSensors(object):
    cdef c_data.MultiRobotSensors * impl
    cdef cppbool is_writable

cdef MultiRobotSensors MultiRobotSensorsFromRef(c_data.MultiRobotSensors &)
cdef MultiRobotSensors MultiRobotSensorsFromCRef(const c_data.MultiRobotSensors &)

