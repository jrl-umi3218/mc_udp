/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#include <mc_udp/server/Server.h>

#include <mc_udp/logging.h>

#include <chrono>
#ifndef WIN32
#  include <unistd.h>
#endif

int main(int argc, char * argv[])
{
  int port = 4444;
  if(argc > 1)
  {
    port = std::atoi(argv[1]);
  }
  mc_udp::Server server(port);
  auto & sensors = server.sensors().messages["dummy"];
  sensors.encoders = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  sensors.torques = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110};
  double reading[6];
  for(size_t i = 0; i < 6; ++i)
  {
    reading[i] = i + 1;
  }
  sensors.fsensor("rfsensor", reading);
  sensors.fsensor("lfsensor", reading);
  sensors.fsensor("rhsensor", reading);
  sensors.fsensor("lhsensor", reading);
  sensors.orientation[0] = 1;
  sensors.orientation[1] = 2;
  sensors.orientation[2] = 3;
  sensors.angularVelocity[0] = 4;
  sensors.angularVelocity[1] = 5;
  sensors.angularVelocity[2] = 6;
  sensors.angularAcceleration[0] = 7;
  sensors.angularAcceleration[1] = 8;
  sensors.angularAcceleration[2] = 9;
  sensors.id = 0;
  size_t control_id = 0;
  while(1)
  {
    auto start_t = std::chrono::system_clock::now();
    server.send();
    auto send_t = std::chrono::system_clock::now();
    if(server.recv())
    {
      const auto & control = server.control().messages.at("dummy");
      control_id = control.id;
      if(control.id != sensors.id - 1)
      {
        MC_UDP_WARNING("[dummy] Server control id " << control.id << " does not match sensors id " << sensors.id)
      }
    }
    auto recv_t = std::chrono::system_clock::now();
    std::chrono::duration<double> send_dt = send_t - start_t;
    std::chrono::duration<double> recv_dt = recv_t - send_t;
    std::cout << "send_dt: " << send_dt.count() * 1000 << ", recv_dt: " << recv_dt.count() * 1000
              << ", control_id: " << control_id << "\n";
    sensors.id += 1;
#ifndef WIN32
    usleep(5000);
#else
    Sleep(500);
#endif
  }
  return 0;
}
