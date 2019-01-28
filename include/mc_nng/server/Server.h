#pragma once

#include <mc_nng/data/RobotControl.h>
#include <mc_nng/data/RobotSensors.h>

#include <netinet/in.h>
#include <sys/socket.h>

namespace mc_nng
{

/** Implement a simple NNG server of sending robot sensors' data and receiving
 * control data
 *
 *  Filling the sensors' data and making sure of the time-consistency of
 *  sensors' and control data is left to clients of this class
 *
 */
struct Server {
  /** Create a non-working server
   *
   * Serving must be started with restart
   */
  Server();

  /** Create the server
   *
   * \param uri URI to bind to
   *
   * \param timeout Reception timeout (microseconds)
   */
  Server(int port, int timeout);

  /** Destructor */
  ~Server();

  /** Receive control data
   *
   * Returns false if no data was received before timeout
   *
   * The server does not take care of checking sensor/control consistency
   */
  bool recv();

  /** Send sensors data
   *
   * Does not modify the sensors id
   */
  void send();

  inline const RobotControl & control() const { return control_; }

  inline RobotSensors & sensors() { return sensors_; }

  /** Stop the server */
  void stop();

  /** Restart server */
  void restart(int port, int timeout);
private:
  RobotControl control_;
  RobotSensors sensors_;
  int socket_;
  sockaddr_in client_;
  socklen_t clientAddrLen_;
  int timeout_;
  std::vector<uint8_t> recvData_;
  std::vector<uint8_t> sendData_;
  void start(int port, int timeout_);
};

}
