#include <mc_udp/client/Client.h>

#include <mc_udp/data/Hello.h>
#include <mc_udp/data/Init.h>
#include <mc_udp/logging.h>

#include <stdexcept>

#include <netdb.h>
#include <string.h>
#include <unistd.h>

namespace mc_udp
{

Client::Client(const std::string & host, int port)
: recvData_(1024, 0),
  sendData_(1024, 0)
{
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(socket_ < 0)
  {
    MC_UDP_ERROR_AND_THROW(std::runtime_error, "Failed to create socket: " << strerror(errno))
  }
  auto hp = gethostbyname(host.c_str());
  if(!hp)
  {
    MC_UDP_ERROR_AND_THROW(std::runtime_error, "Faile to resolve host (" << host << "): " << strerror(h_errno));
  }
  memset(&server_, 0, sizeof(server_));
  server_.sin_family = AF_INET;
  server_.sin_port = htons(port);
  memcpy(&server_.sin_addr, hp->h_addr_list[0], hp->h_length);
  sendHello();
}

bool Client::recv()
{
  int sz = recvfrom(socket_, recvData_.data(), recvData_.size(), MSG_DONTWAIT, NULL, NULL);
  if(sz >= static_cast<int>(recvData_.size()))
  {
    MC_UDP_WARNING("Receive buffer was too small to get it all")
    recvData_.resize(recvData_.size() * 2);
  }
  else if(sz > 0)
  {
    sensors_.fromBuffer(recvData_.data());
    do
    {
      sz = recvfrom(socket_, recvData_.data(), recvData_.size(), MSG_DONTWAIT, NULL, NULL);
      if(sz > 0)
      {
        sensors_.fromBuffer(recvData_.data());
        MC_UDP_INFO("Had more data waiting reception " << sensors_.id)
      }
    } while(sz > 0);
    return true;
  }
  return false;
}

void Client::sendHello()
{
  sendto(socket_, &Hello, sizeof(Hello), 0, (struct sockaddr*)&server_, sizeof(server_));
}

void Client::init()
{
  sendto(socket_, &Init, sizeof(Init), 0, (struct sockaddr*)&server_, sizeof(server_));
}

void Client::send()
{
  /** Otherwise send regular data */
  auto sz = control_.size();
  if(sz > sendData_.size())
  {
    sendData_.resize(2*sendData_.size());
    MC_UDP_INFO("Needed to resize sending buffer")
  }
  control_.toBuffer(sendData_.data());
  sendto(socket_, sendData_.data(), sz, 0, (struct sockaddr*)&server_, sizeof(server_));
}

}
