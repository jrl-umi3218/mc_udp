#include <mc_nng/client/Client.h>
#include <mc_nng/logging.h>

int main(int argc, char * argv[])
{
  std::string uri = "tcp://localhost:4444";
  int timeout = 4;
  if(argc > 1)
  {
    uri = argv[1];
  }
  if(argc > 2)
  {
    timeout = std::atoi(argv[2]);
  }
  mc_nng::Client client(uri, timeout);
  auto & control = client.control();
  control.encoders = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  uint64_t prev_id = 0;
  while(1)
  {
    if(client.recv())
    {
      if(client.sensors().id != prev_id + 1)
      {
        MC_NNG_WARNING("[dummy] Missed one frame of sensors data (id: " << client.sensors().id << ", previous id: " << prev_id << ")")
      }
      control.id = client.sensors().id;
      prev_id = control.id;
      //for(const auto & fs : client.sensors().fsensors)
      //{
      //  std::cout << "- " << fs.name << ": " << fs.reading[0] << ", "
      //                                       << fs.reading[1] << ", "
      //                                       << fs.reading[2] << ", "
      //                                       << fs.reading[3] << ", "
      //                                       << fs.reading[4] << ", "
      //                                       << fs.reading[5] << "\n";
      //}
      std::cout << "id: " << control.id << " encoders size: " << client.control().encoders.size() << "\n";
      client.send();
    }
  }
  return 0;
}
