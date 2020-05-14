#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

from libcpp.string cimport string
from libcpp.unordered_map cimport unordered_map
from libcpp.vector cimport vector

from libc.stdint cimport uint64_t

cdef extern from "<mc_udp/data/RobotSensors.h>" namespace "mc_udp":
    cdef cppclass ForceSensor:
        string name
        double reading[6]

    cdef cppclass RobotSensors:
        uint64_t id
        vector[double] encoders
        vector[double] encoderVelocities
        vector[double] torques
        vector[ForceSensor] fsensors
        double orientation[3]
        double angularVelocity[3]
        double angularAcceleration[3]
        double position[3]
        double floatingBasePos[3]
        double floatingBaseRPY[3]
        double floatingBaseVel[6]
        double floatingBaseAcc[6]
        void fsensor(string, double[6])

cdef extern from "<mc_udp/data/RobotControl.h>" namespace "mc_udp":
    cdef cppclass RobotControl:
        uint64_t id
        vector[double] encoders
        vector[double] encoderVelocities

cdef extern from "<mc_udp/data/MultiRobotMessage.h>" namespace "mc_udp":
    cdef cppclass MultiRobotMessage[T]:
        unordered_map[string, T] messages

    ctypedef MultiRobotMessage[RobotControl] MultiRobotControl
    ctypedef MultiRobotMessage[RobotSensors] MultiRobotSensors

ctypedef ForceSensor * ForceSensorPtr
ctypedef RobotSensors * RobotSensorsPtr
ctypedef RobotControl * RobotControlPtr
ctypedef MultiRobotSensors * MultiRobotSensorsPtr
ctypedef MultiRobotControl * MultiRobotControlPtr
