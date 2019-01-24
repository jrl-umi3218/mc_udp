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
struct Client {
  /** Create the server
   *
   * \param uri URI to bind to
   *
   * \param timeout Reception timeout
   */
  Client(const std::string & uri, int timeout);

  /** Receive sensors data
   *
   * Returns false if no data was received before timeout
   *
   * The server does not take care of checking sensor/control consistency
   */
  bool recv();

  /** Send control data
   *
   * Does not modify the control id
   */
  void send();

  inline RobotControl & control() { return control_; }

  inline const RobotSensors & sensors() const { return sensors_; }
private:
  RobotControl control_;
  RobotSensors sensors_;
  nng_socket socket_;
};

}
