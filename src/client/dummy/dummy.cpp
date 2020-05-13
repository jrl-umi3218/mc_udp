/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#include <mc_udp/client/Client.h>
#include <mc_udp/logging.h>

int main(int argc, char * argv[])
{
  std::string host = "localhost";
  int port = 4444;
  if(argc > 1)
  {
    host = argv[1];
  }
  if(argc > 2)
  {
    port = std::atoi(argv[2]);
  }
  mc_udp::Client client(host, port);
  auto & control = client.control().messages["dummy"];
  control.encoders = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  uint64_t prev_id = 0;
  bool init = false;
  while(1)
  {
    if(client.recv())
    {
      if(!init)
      {
        init = true;
        client.init();
      }
      const auto & sensors = client.sensors().messages.at("dummy");
      if(sensors.id != prev_id + 1)
      {
        MC_UDP_WARNING("[dummy] Missed one frame of sensors data (id: " << sensors.id << ", previous id: " << prev_id
                                                                        << ")")
      }
      control.id = sensors.id;
      prev_id = control.id;
      // for(const auto & fs : client.sensors().fsensors)
      //{
      //  std::cout << "- " << fs.name << ": " << fs.reading[0] << ", "
      //                                       << fs.reading[1] << ", "
      //                                       << fs.reading[2] << ", "
      //                                       << fs.reading[3] << ", "
      //                                       << fs.reading[4] << ", "
      //                                       << fs.reading[5] << "\n";
      //}
      std::cout << "sensors.id: " << sensors.id << "\n";
      client.send();
    }
  }
  return 0;
}
