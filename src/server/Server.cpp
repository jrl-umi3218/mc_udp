/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#include <mc_udp/server/Server.h>

#include <mc_udp/data/Hello.h>
#include <mc_udp/data/Init.h>
#include <mc_udp/logging.h>

#include <errno.h>
#include <stdexcept>
#include <string.h>

#ifndef WIN32
#  include <unistd.h>
#endif

namespace mc_udp
{

Server::Server() : socket_(0), recvData_(1024, 0), sendData_(1024, 0), initClient_(false), waitInit_(false)
{
  init();
}

Server::Server(int port)
{
  init();
  start(port);
}

void Server::init()
{
#ifdef WIN32
  WSAStartup(MAKEWORD(2, 2), &wsaData_);
#endif
}

Server::~Server()
{
  stop();
#ifdef WIN32
  WSACleanup();
#endif
}

bool Server::recv()
{
  int length =
#ifndef WIN32
      recvfrom(socket_, recvData_.data(), recvData_.size(), MSG_DONTWAIT, (struct sockaddr *)&client_, &clientAddrLen_);
#else
      recvfrom(socket_, (char *)recvData_.data(), recvData_.size(), 0, (struct sockaddr *)&client_, &clientAddrLen_);
#endif
  if(length > 0)
  {
    if(length == sizeof(Hello) * sizeof(uint8_t))
    {
      MC_UDP_INFO(id_ << " New client sending data")
      initClient_ = true;
      waitInit_ = true;
    }
    else if(length == sizeof(Init) * sizeof(uint8_t))
    {
      MC_UDP_INFO(id_ << " Start streaming data to client")
      for(auto it = sensors().messages.begin(); it != sensors().messages.end(); it++)
      {
        it->second.id = 0;
      }
      initClient_ = false;
    }
    else if(length >= static_cast<int>(recvData_.size()))
    {
      MC_UDP_WARNING(id_ << " Received exactly the buffer size, resizing for next round")
      recvData_.resize(2 * recvData_.size());
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
    MC_UDP_WARNING(id_ << " Send data buffer is too small for required sending (size: " << sendData_.size()
                       << ", required: " << sz << ")")
    sendData_.resize(sz);
  }
  sensors_.toBuffer(sendData_.data());
  if((initClient_ && waitInit_) || !initClient_)
  {
    waitInit_ = false;
#ifndef WIN32
    sendto(socket_, sendData_.data(), sz, 0, (struct sockaddr *)&client_, clientAddrLen_);
#else
    sendto(socket_, (const char *)sendData_.data(), sz, 0, (struct sockaddr *)&client_, clientAddrLen_);
#endif
  }
}

void Server::stop()
{
  if(socket_ != 0)
  {
#ifndef WIN32
    close(socket_);
#else
    closesocket(socket_);
#endif
  }
}

void Server::restart(int port)
{
  stop();
  start(port);
}

void Server::start(int port)
{
  std::stringstream ss;
  ss << "[UDP::" << port << "]";
  id_ = ss.str();
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(socket_ < 0)
  {
    MC_UDP_ERROR_AND_THROW(std::runtime_error, "Failed to create socket: " << strerror(errno))
  }
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  int err = bind(socket_, (struct sockaddr *)&addr, sizeof(addr));
#ifdef WIN32
  u_long iMode = 1;
  if(ioctlsocket(socket_, FIONBIO, &iMode) != NO_ERROR)
  {
    MC_UDP_ERROR_AND_THROW(std::runtime_error, "Failed to set FIONBIO on socket: " << strerror(errno))
  }
#endif
  if(err < 0)
  {
    MC_UDP_ERROR_AND_THROW(std::runtime_error, "Failed bind the socket: " << strerror(errno))
  }
  clientAddrLen_ = sizeof(client_);
}

} // namespace mc_udp
