mc\_rtc udp-based client/server
==

This package is meant as a replacement for [mc\_rtc\_tcp](https://gite.lirmm.fr/multi-contact/mc_rtc_tcp).

You don't need to build both the client and server(s) on the same machine, the project will build what it can.

Client
==

The client is a regular mc\_rtc interface that gets data through the UDP socket.

Requirements
--

- [mc\_rtc](https://gite.lirmm.fr/multi-contact/mc_rtc)

Usage
--

```bash
$ MCUDPControl [config.conf]
```

The default configuration file is ``~/.config/mc_rtc/mc_rtc.conf``. You should
set up the robot hostname or URI in that file:

```
"UDP":
{
  "host": "hrp4005c",
  "port": 4444
}
```

Server(s)
==

For now a single server is implemented, it is a simple OpenRTM component.

Requirements
--

- [hrpsys-base](https://github.com/fkanehiro/hrpsys-base)
