mc\_rtc nng-based client/server
==

This package is meant as a replacement for [mc\_rtc\_tcp](https://gite.lirmm.fr/multi-contact/mc_rtc_tcp).

It uses an [NNG pair](https://nanomsg.org/gettingstarted/nng/pair.html) as a basis for communication between the client and the server. For simplicity sake, NNG is provided as submodule of this repository.

You don't need to build both the client and server(s) on the same machine, the project will build what it can.

Client
==

The client is a regular mc\_rtc interface that gets data through the NNG socket.

Requirements
--

- [mc\_rtc](https://gite.lirmm.fr/multi-contact/mc_rtc)

Usage
--

```bash
$ MCControlNNG [config.conf]
```

The default configuration file is ``~/.config/mc_rtc/mc_rtc.conf``.

Server(s)
==

For now a single server is implemented, it is a simple OpenRTM component.

Requirements
--

- [hrpsys-base](https://github.com/fkanehiro/hrpsys-base)
