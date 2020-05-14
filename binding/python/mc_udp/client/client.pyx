# distutils: language = c++

#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport c_client

cimport mc_udp.data.data as data

cdef class Client(object):
    def __dealloc__(self):
        del self.impl
    def __cinit__(self, host, port):
        if isinstance(host, unicode):
            host = host.encode(u'ascii')
        self.impl = new c_client.Client(host, port)
    def init(self):
        self.impl.init()
    def recv(self):
        return self.impl.recv()
    def send(self):
        self.impl.send()
    def control(self):
        return data.MultiRobotControlFromRef(self.impl.control())
    def sensors(self):
        return data.MultiRobotSensorsFromCRef(self.impl.sensors())
