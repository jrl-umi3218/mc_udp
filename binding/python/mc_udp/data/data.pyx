# distutils: language = c++

#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport c_data
cimport c_data_private

from cython.operator cimport dereference as deref, preincrement as inc

from libcpp.cast cimport const_cast
from libcpp.unordered_map cimport unordered_map

cdef class ForceSensor(object):
    def __cinit__(self):
        self.impl = NULL
        self.is_writable = False
    def __valid(self):
        assert self.impl, "ForceSensor cannot be created by Python"
    def __writable(self):
        self.__valid()
        assert self.is_writable, "ForceSensor instance is not writable"
    property name:
        def __get__(self):
            self.__valid()
            return self.impl.name
        def __set__(self, name):
            self.__writable()
            self.impl.name = name
    property reading:
        def __get__(self):
            self.__valid()
            return self.impl.reading
        def __set__(self, reading):
            self.__writable()
            self.impl.reading = reading

cdef ForceSensor ForceSensorFromRef(c_data.ForceSensor & fs):
    cdef ForceSensor ret = ForceSensor()
    ret.impl = &fs
    ret.is_writable = True
    return ret

cdef ForceSensor ForceSensorFromCRef(const c_data.ForceSensor & fs):
    cdef ForceSensor ret = ForceSensor()
    ret.impl = const_cast[c_data.ForceSensorPtr](&fs)
    ret.is_writable = False
    return ret

cdef class RobotSensors(object):
    def __cinit__(self):
        self.impl = NULL
        self.is_writable = False
    def __valid(self):
        assert self.impl, "RobotSensors cannot be created by Python"
    def __writable(self):
        self.__valid()
        assert self.is_writable, "RobotSensors instance is not writable"
    property id:
        def __get__(self):
            self.__valid()
            return self.impl.id
        def __set__(self, id):
            self.__writable()
            self.impl.id = id
    property encoders:
        def __get__(self):
            self.__valid()
            return self.impl.encoders
        def __set__(self, encoders):
            self.__writable()
            self.impl.encoders = encoders
    property encoderVelocities:
        def __get__(self):
            self.__valid()
            return self.impl.encoderVelocities
        def __set__(self, encoderVelocities):
            self.__writable()
            self.impl.encoderVelocities = encoderVelocities
    property torques:
        def __get__(self):
            self.__valid()
            return self.impl.torques
        def __set__(self, torques):
            self.__writable()
            self.impl.torques = torques
    property fsensors:
        def __get__(self):
            self.__valid()
            return [ForceSensorFromCRef(self.impl.fsensors.at(i)) for i in range(self.impl.fsensors.size())]
    property orientation:
        def __get__(self):
            self.__valid()
            return self.impl.orientation
        def __set__(self, value):
            self.__writable()
            self.impl.orientation = value
    property angularVelocity:
        def __get__(self):
            self.__valid()
            return self.impl.angularVelocity
        def __set__(self, value):
            self.__writable()
            self.impl.angularVelocity = value
    property angularAcceleration:
        def __get__(self):
            self.__valid()
            return self.impl.angularAcceleration
        def __set__(self, value):
            self.__writable()
            self.impl.angularAcceleration = value
    property position:
        def __get__(self):
            self.__valid()
            return self.impl.position
        def __set__(self, value):
            self.__writable()
            self.impl.position = value
    property floatingBasePos:
        def __get__(self):
            self.__valid()
            return self.impl.floatingBasePos
        def __set__(self, value):
            self.__writable()
            self.impl.floatingBasePos = value
    property floatingBaseRPY:
        def __get__(self):
            self.__valid()
            return self.impl.floatingBaseRPY
        def __set__(self, value):
            self.__writable()
            self.impl.floatingBaseRPY = value
    property floatingBaseVel:
        def __get__(self):
            self.__valid()
            return self.impl.floatingBaseVel
        def __set__(self, value):
            self.__writable()
            self.impl.floatingBaseVel = value
    property floatingBaseAcc:
        def __get__(self):
            self.__valid()
            return self.impl.floatingBaseAcc
        def __set__(self, value):
            self.__writable()
            self.impl.floatingBaseAcc = value
    def fsensor(self, name, data):
        assert len(data) == 6, "Force sensor data must have 6 elements exactly"
        cdef double data_c[6]
        for i in range(6):
            data_c[i] = data[i]
        if isinstance(name, unicode):
            name = name.encode(u'ascii')
        self.impl.fsensor(name, data_c)

cdef RobotSensors RobotSensorsFromRef(c_data.RobotSensors & fs):
    cdef RobotSensors ret = RobotSensors()
    ret.impl = &fs
    ret.is_writable = True
    return ret

cdef RobotSensors RobotSensorsFromCRef(const c_data.RobotSensors & fs):
    cdef RobotSensors ret = RobotSensors()
    ret.impl = const_cast[c_data.RobotSensorsPtr](&fs)
    ret.is_writable = False
    return ret

cdef class RobotControl(object):
    def __cinit__(self):
        self.impl = NULL
        self.is_writable = False
    def __valid(self):
        assert self.impl, "RobotControl cannot be created by Python"
    def __writable(self):
        self.__valid()
        assert self.is_writable, "RobotControl instance is not writable"
    property id:
        def __get__(self):
            self.__valid()
            return self.impl.id
        def __set__(self, id):
            self.__writable()
            self.impl.id = id
    property encoders:
        def __get__(self):
            self.__valid()
            return self.impl.encoders
        def __set__(self, encoders):
            self.__writable()
            self.impl.encoders = encoders
    property encoderVelocities:
        def __get__(self):
            self.__valid()
            return self.impl.encoderVelocities
        def __set__(self, encoderVelocities):
            self.__writable()
            self.impl.encoderVelocities = encoderVelocities

cdef RobotControl RobotControlFromRef(c_data.RobotControl & fs):
    cdef RobotControl ret = RobotControl()
    ret.impl = &fs
    ret.is_writable = True
    return ret

cdef RobotControl RobotControlFromCRef(const c_data.RobotControl & fs):
    cdef RobotControl ret = RobotControl()
    ret.impl = const_cast[c_data.RobotControlPtr](&fs)
    ret.is_writable = False
    return ret

cdef class _ControlMapWrapper(object):
    def __cinit__(self):
        self.impl = NULL
        self.is_writable = False
    def keys(self):
        return c_data_private.getKeys[c_data.RobotControl](self.impl)
    def count(self, key):
        if isinstance(key, unicode):
            key = key.encode(u'ascii')
        return self.impl.count(key)
    def __getitem__(self, key):
        assert self.impl, "This must be obtained from C"
        if isinstance(key, unicode):
            key = key.encode(u'ascii')
        if self.is_writable:
            return RobotControlFromRef(c_data_private.getControl(self.impl, key))
        else:
            return RobotControlFromCRef(<const c_data.RobotControl &>(self.impl.at(key)))

ctypedef unordered_map[string, c_data.RobotControl] * _ControlMapPtr

cdef _ControlMapWrapper _ControlMapFromRef(unordered_map[string, c_data.RobotControl] & m):
    cdef _ControlMapWrapper ret = _ControlMapWrapper()
    ret.impl = &m
    ret.is_writable = True
    return ret

cdef _ControlMapWrapper _ControlMapFromCRef(const unordered_map[string, c_data.RobotControl] & m):
    cdef _ControlMapWrapper ret = _ControlMapWrapper()
    ret.impl = const_cast[_ControlMapPtr](&m)
    ret.is_writable = False
    return ret

cdef class MultiRobotControl(object):
    def __cinit__(self):
        self.impl = NULL
        self.is_writable = False
    def __valid(self):
        assert self.impl, "MultiRobotControl cannot be created by Python"
    def __writable(self):
        self.__valid()
        assert self.is_writable, "MultiRobotControl instance is not writable"
    property messages:
        def __get__(self):
            self.__valid()
            if self.is_writable:
                return _ControlMapFromRef(self.impl.messages)
            else:
                return _ControlMapFromCRef(self.impl.messages)

cdef MultiRobotControl MultiRobotControlFromRef(c_data.MultiRobotControl & fs):
    cdef MultiRobotControl ret = MultiRobotControl()
    ret.impl = &fs
    ret.is_writable = True
    return ret

cdef MultiRobotControl MultiRobotControlFromCRef(const c_data.MultiRobotControl & fs):
    cdef MultiRobotControl ret = MultiRobotControl()
    ret.impl = const_cast[c_data.MultiRobotControlPtr](&fs)
    ret.is_writable = False
    return ret

cdef class _SensorsMapWrapper(object):
    def __cinit__(self):
        self.impl = NULL
        self.is_writable = False
    def keys(self):
        return c_data_private.getKeys[c_data.RobotSensors](self.impl)
    def count(self, key):
        if isinstance(key, unicode):
            key = key.encode(u'ascii')
        return self.impl.count(key)
    def __getitem__(self, key):
        assert self.impl, "This must be obtained from C"
        if isinstance(key, unicode):
            key = key.encode(u'ascii')
        if self.is_writable:
            return RobotSensorsFromRef(c_data_private.getSensors(self.impl, key))
        else:
            return RobotSensorsFromCRef(<const c_data.RobotSensors &>(self.impl.at(key)))

ctypedef unordered_map[string, c_data.RobotSensors] * _SensorsMapPtr

cdef _SensorsMapWrapper _SensorsMapFromRef(unordered_map[string, c_data.RobotSensors] & m):
    cdef _SensorsMapWrapper ret = _SensorsMapWrapper()
    ret.impl = &m
    ret.is_writable = True
    return ret

cdef _SensorsMapWrapper _SensorsMapFromCRef(const unordered_map[string, c_data.RobotSensors] & m):
    cdef _SensorsMapWrapper ret = _SensorsMapWrapper()
    ret.impl = const_cast[_SensorsMapPtr](&m)
    ret.is_writable = False
    return ret

cdef class MultiRobotSensors(object):
    def __cinit__(self):
        self.impl = NULL
        self.is_writable = False
    def __valid(self):
        assert self.impl, "MultiRobotSensors cannot be created by Python"
    def __writable(self):
        self.__valid()
        assert self.is_writable, "MultiRobotSensors instance is not writable"
    property messages:
        def __get__(self):
            self.__valid()
            if self.is_writable:
                return _SensorsMapFromRef(self.impl.messages)
            else:
                return _SensorsMapFromCRef(self.impl.messages)

cdef MultiRobotSensors MultiRobotSensorsFromRef(c_data.MultiRobotSensors & fs):
    cdef MultiRobotSensors ret = MultiRobotSensors()
    ret.impl = &fs
    ret.is_writable = True
    return ret

cdef MultiRobotSensors MultiRobotSensorsFromCRef(const c_data.MultiRobotSensors & fs):
    cdef MultiRobotSensors ret = MultiRobotSensors()
    ret.impl = const_cast[c_data.MultiRobotSensorsPtr](&fs)
    ret.is_writable = False
    return ret

