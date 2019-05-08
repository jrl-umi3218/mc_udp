#pragma once

#include <mc_udp/data/RobotControl.h>
#include <mc_udp/data/RobotSensors.h>

#include <netinet/in.h>
#include <sys/socket.h>

namespace mc_udp
{

/** Implement a simple UDP server of sending robot sensors' data and receiving
 * control data
 *
 *  Filling the sensors' data and making sure of the time-consistency of
 *  sensors' and control data is left to clients of this class
 *
 */
struct Server
{
  /** Create a non-working server
   *
   * Serving must be started with restart
   */
  Server();

  /** Create the server
   *
   * \param uri URI to bind to
   */
  Server(int port);

  /** Destructor */
  ~Server();

  /** Receive control data
   *
   * Returns false if no data was received.
   *
   * The server does not take care of checking sensor/control consistency
   */
  bool recv();

  /** Send sensors data
   *
   * Does not modify the sensors id
   */
  void send();

  inline const RobotControl & control() const
  {
    return control_;
  }

  inline RobotSensors & sensors()
  {
    return sensors_;
  }

  /** Stop the server */
  void stop();

  /** Restart server */
  void restart(int port);

private:
  RobotControl control_;
  RobotSensors sensors_;
  int socket_;
  sockaddr_in client_;
  socklen_t clientAddrLen_;
  std::vector<uint8_t> recvData_;
  std::vector<uint8_t> sendData_;
  void start(int port);
  bool initClient_;
  bool waitInit_;
  std::string id_;
};

} // namespace mc_udp
