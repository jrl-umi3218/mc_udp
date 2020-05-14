#!/usr/bin/env python

#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

from __future__ import print_function

import mc_udp
import sys
import time

if __name__ == "__main__":
    host = "localhost"
    port = 4444
    if len(sys.argv) > 1:
        host = sys.argv[1]
    if len(sys.argv) > 2:
        port = int(sys.argv[2])
    client = mc_udp.client.Client(host, port)
    control = client.control().messages["dummy"]
    control.encoders = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
    prev_id = 0
    while not client.recv():
        pass
    client.init()
    client.send()
    iter_ = 0
    while True:
        if client.recv():
            sensors = client.sensors().messages["dummy"]
            if sensors.id != prev_id + 1:
                print("[dummy] Missed one frame of sensors data (id: {}, previous id: {})".format(sensors.id, prev_id))
            control.id = sensors.id
            prev_id = control.id
            if iter_ % 200 == 0:
                print("sensors.id: {}".format(sensors.id))
                print("sensors.encoders: {}".format(sensors.encoders))
                print("sensors.torques: {}".format(sensors.torques))
                print("sensors.fsensors: {}".format({fs.name: fs.reading for fs in sensors.fsensors}))
                print("sensors.orientation: {}".format(sensors.orientation))
                print("sensors.angularVelocity: {}".format(sensors.angularVelocity))
                print("sensors.angularAcceleration: {}".format(sensors.angularAcceleration))
            iter_ += 1
            client.send()
            time.sleep(0.001)
