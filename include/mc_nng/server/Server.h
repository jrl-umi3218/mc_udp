#pragma once

#include <mc_nng/data/RobotControl.h>
#include <mc_nng/data/RobotSensors.h>

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>

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
  Server() = default;

  /** Create the server
   *
   * \param uri URI to bind to
   *
   * \param timeout Reception timeout
   */
  Server(const std::string & uri, int timeout);

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
  void restart(const std::string & uri, int timeout);
private:
  RobotControl control_;
  RobotSensors sensors_;
  nng_socket socket_;
  void start(const std::string & uri, int timeout);
};

}