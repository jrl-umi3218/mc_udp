#!/usr/bin/env python

#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

from __future__ import print_function

import mc_udp
import sys
import time

if __name__ == "__main__":
    port = 4444
    if len(sys.argv) > 1:
        port = int(sys.argv[1])
    server = mc_udp.Server(port)
    sensors = server.sensors().messages["dummy"]
    sensors.encoders = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    sensors.torques = [100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110]
    reading = [1, 2, 3, 4, 5, 6]
    [ sensors.fsensor(sensor, reading) for sensor in ["rfsensor", "lfsensor", "rhsensor", "lhsensor"] ]
    sensors.orientation = [1, 2, 3]
    sensors.angularVelocity = [4, 5, 6]
    sensors.linearAcceleration = [7, 8, 9]
    sensors.id = 0
    control_id = 0
    iter_ = 0
    while True:
        server.send()
        if server.recv():
            control = server.control().messages["dummy"]
            if control.id != sensors.id - 1:
                print("[dummy] Server control id {} does not match sensors id".format(control.id, sensors.id))
        sensors.id += 1
        time.sleep(0.005)
