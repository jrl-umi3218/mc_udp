/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#include <mc_udp/client/Client.h>

#include <mc_udp/data/Hello.h>
#include <mc_udp/data/Init.h>
#include <mc_udp/logging.h>

#include <stdexcept>
#include <string.h>

#ifndef WIN32
#  include <netdb.h>
#  include <unistd.h>
#endif

namespace mc_udp
{

Client::Client(const std::string & host, int port) : recvData_(2048, 0), sendData_(2048, 0)
{
#ifdef WIN32
  WSAStartup(MAKEWORD(2, 2), &wsaData_);
#endif
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
#ifdef WIN32
  u_long iMode = 1;
  if(ioctlsocket(socket_, FIONBIO, &iMode) != NO_ERROR)
  {
    MC_UDP_ERROR_AND_THROW(std::runtime_error, "Failed to set FIONBIO on socket: " << strerror(errno))
  }
#endif
  sendHello();
}

Client::~Client()
{
  if(socket_ != 0)
  {
#ifndef WIN32
    close(socket_);
#else
    closesocket(socket_);
#endif
  }
#ifdef WIN32
  WSACleanup();
#endif
}

bool Client::recv()
{
#ifndef WIN32
  int sz = recvfrom(socket_, recvData_.data(), recvData_.size(), MSG_DONTWAIT, NULL, NULL);
#else
  int sz = recvfrom(socket_, (char *)recvData_.data(), recvData_.size(), 0, NULL, NULL);
#endif
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
#ifndef WIN32
      sz = recvfrom(socket_, recvData_.data(), recvData_.size(), MSG_DONTWAIT, NULL, NULL);
#else
      sz = recvfrom(socket_, (char *)recvData_.data(), recvData_.size(), 0, NULL, NULL);
#endif
      if(sz > 0)
      {
        sensors_.fromBuffer(recvData_.data());
        MC_UDP_INFO("Had more data waiting reception " << sensors_.messages.begin()->second.id)
      }
    } while(sz > 0);
    return true;
  }
  return false;
}

void Client::sendHello()
{
#ifndef WIN32
  sendto(socket_, &Hello, sizeof(Hello), 0, (struct sockaddr *)&server_, sizeof(server_));
#else
  sendto(socket_, (const char *)&Hello, sizeof(Hello), 0, (struct sockaddr *)&server_, sizeof(server_));
#endif
}

void Client::init()
{
#ifndef WIN32
  sendto(socket_, &Init, sizeof(Init), 0, (struct sockaddr *)&server_, sizeof(server_));
#else
  sendto(socket_, (const char *)&Init, sizeof(Init), 0, (struct sockaddr *)&server_, sizeof(server_));
#endif
}

void Client::send()
{
  /** Otherwise send regular data */
  auto sz = control_.size();
  if(sz > sendData_.size())
  {
    sendData_.resize(2 * sendData_.size());
    MC_UDP_INFO("Needed to resize sending buffer")
  }
  control_.toBuffer(sendData_.data());
#ifndef WIN32
  sendto(socket_, sendData_.data(), sz, 0, (struct sockaddr *)&server_, sizeof(server_));
#else
  sendto(socket_, (const char *)sendData_.data(), sz, 0, (struct sockaddr *)&server_, sizeof(server_));
#endif
}

} // namespace mc_udp
