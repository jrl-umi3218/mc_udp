mc\_rtc UDP-based client/server
==

You don't need to build both the client and server(s) on the same machine, the project will build what it can.

Client
==

The client is a regular mc\_rtc interface that gets data through the UDP socket.

Requirements
--

- [mc\_rtc](https://github.com/jrl-umi3218/mc_rtc)

Usage
--

For help on the usage of the program, simply run:

```bash
$ MCUDPControl --help
```

The default configuration file is ``~/.config/mc_rtc/mc_rtc.yaml``.

The program will look up your mc\_rtc configuration for udp configuration, it's
specifically looking for a `UDP` section as follows:

```
UDP:
  host: hrp4005c
  port: 4444
```

Those can be overriden by providing options to the program.

Server(s)
==

For now a single server is implemented, it's composed of two OpenRTM components:

- `MCUDPSensors`: reads sensors data from OpenRTM input ports and send them to
  the connected client

- `MCUDPControl`: receives control input from the network and send them through
  an OpenRTM output port

Requirements
--

- [hrpsys-base](https://github.com/fkanehiro/hrpsys-base)
