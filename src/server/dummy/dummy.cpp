#include <mc_nng/server/Server.h>
#include <mc_nng/logging.h>

#include <chrono>
#include <unistd.h>

int main(int argc, char * argv[])
{
  std::string uri = "tcp://*:4444";
  int timeout = 4;
  if(argc > 1)
  {
    uri = argv[1];
  }
  if(argc > 2)
  {
    timeout = std::atoi(argv[2]);
  }
  mc_nng::Server server(uri, timeout);
  auto & sensors = server.sensors();
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
  server.sensors().id = 0;
  while(1)
  {
    auto start_t = std::chrono::system_clock::now();
    server.send();
    auto send_t = std::chrono::system_clock::now();
    if(server.recv())
    {
      if(server.control().id != server.sensors().id)
      {
        MC_NNG_WARNING("[dummy] Server control id " << server.control().id << " does not match sensors id " << server.sensors().id)
      }
    }
    auto recv_t = std::chrono::system_clock::now();
    std::chrono::duration<double> send_dt = send_t - start_t;
    std::chrono::duration<double> recv_dt = recv_t - send_t;
    std::cout << "send_dt: " << send_dt.count()*1000 << ", recv_dt: " << recv_dt.count() * 1000 << "\n";
    server.sensors().id += 1;
  }
  return 0;
}
