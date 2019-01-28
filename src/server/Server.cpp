#include <mc_nng/server/Server.h>
#include <mc_nng/data/Hello.h>

#include <mc_nng/logging.h>

#include <stdexcept>

#include <string.h>
#include <unistd.h>

namespace mc_nng
{

Server::Server()
: recvData_(1024, 0), sendData_(1024,0)
{
}

Server::Server(int port, int timeout)
: recvData_(1024, 0), sendData_(1024,0)
{
  start(port, timeout);
}

Server::~Server()
{
  close(socket_);
}

bool Server::recv()
{
  clientAddrLen_ = sizeof(client_);
  int length = recvfrom(socket_, recvData_.data(), recvData_.size(), MSG_DONTWAIT, (struct sockaddr*)&client_, &clientAddrLen_);
  if(length > 0)
  {
    if(length == sizeof(Hello) * sizeof(uint8_t))
    {
      MC_NNG_INFO("[UDP] New client sending data")
    }
    else if(length >= static_cast<int>(recvData_.size()))
    {
      MC_NNG_WARNING("[UDP] Received exactly the buffer size, resizing for next round")
      recvData_.resize(2*recvData_.size());
    }
    else
    {
      control_.fromBuffer(recvData_.data());
      return true;
    }
  }
  return false;
}

void Server::send()
{
  size_t sz = sensors_.size();
  if(sz > sendData_.size())
  {
    MC_NNG_WARNING("[UDP] Send data buffer is too small for required sending (size: " << sendData_.size() << ", required: " << sz << ")")
    sendData_.resize(sz);
  }
  sensors_.toBuffer(sendData_.data());
  sendto(socket_, sendData_.data(), sz, 0, (struct sockaddr*)&client_, clientAddrLen_);
}

void Server::stop()
{
  close(socket_);
}

void Server::restart(int port, int timeout)
{
  stop();
  start(port, timeout);
}

void Server::start(int port, int timeout)
{
  timeout_ = timeout;
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(socket_ < 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to create socket: " << strerror(errno))
  }
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  int err = bind(socket_, (struct sockaddr*)&addr, sizeof(addr));
  if(err < 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed bind the socket: " << strerror(errno))
  }
  timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = timeout_;
  err = setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  if(err < 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to set recv timeout: " << strerror(errno))
  }
}

}
