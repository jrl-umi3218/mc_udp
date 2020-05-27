# distutils: language = c++

#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport c_server

cimport mc_udp.data.data as data

cdef class Server(object):
    def __dealloc__(self):
        del self.impl
    def __cinit__(self, port = None):
        if port is None:
            self.impl = new c_server.Server()
        else:
            self.impl = new c_server.Server(port)
    def restart(self, port):
        self.impl.restart(port)
    def recv(self):
        return self.impl.recv()
    def send(self):
        self.impl.send()
    def stop(self):
        self.impl.stop()
    def control(self):
        return data.MultiRobotControlFromCRef(self.impl.control())
    def sensors(self):
        return data.MultiRobotSensorsFromRef(self.impl.sensors())
