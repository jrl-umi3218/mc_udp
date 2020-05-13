/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#ifndef WIN32
#  include <netinet/in.h>
#  include <sys/socket.h>
#else
#  include <WS2tcpip.h>
#  include <WinSock2.h>
#  pragma comment(lib, "ws2_32.lib")
#endif

#include <mc_udp/client/api.h>
#include <mc_udp/data/MultiRobotMessage.h>

namespace mc_udp
{

/** Implement a simple UDP server of sending robot sensors' data and receiving
 * control data
 *
 *  Filling the sensors' data and making sure of the time-consistency of
 *  sensors' and control data is left to clients of this class
 *
 */
struct MC_UDP_CLIENT_DLLAPI Client
{
  /** Create the server
   *
   * \param host Host to send messages to
   *
   * \param port Port to send messages to
   */
  Client(const std::string & host, int port);

  /** Destructor */
  ~Client();

  /** Should be send when we are ready to receive more data after the first sensors */
  void init();

  /** Receive sensors data
   *
   * Returns false if no data was received
   *
   * The server does not take care of checking sensor/control consistency
   */
  bool recv();

  /** Send control data
   *
   * Does not modify the control id
   */
  void send();

  inline MultiRobotControl & control()
  {
    return control_;
  }

  inline const MultiRobotSensors & sensors() const
  {
    return sensors_;
  }

private:
  MultiRobotControl control_;
  MultiRobotSensors sensors_;
  int socket_;
  sockaddr_in server_;
  std::vector<uint8_t> recvData_;
  std::vector<uint8_t> sendData_;
  sockaddr_in recvAddr_;
  socklen_t recvAddrLen_;
#ifdef WIN32
  WSAData wsaData_;
#endif

  void sendHello();
};

} // namespace mc_udp
