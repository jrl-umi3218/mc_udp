#pragma once

#include <string>
#include <vector>

namespace mc_udp
{

/** Robot control data sent to the server */
struct RobotControl
{
  /** Unique identifier for the control, should repeat the same as the sensors' reading */
  uint64_t id;
  /** Joint encoders provided in the robot reference order */
  std::vector<double> encoders;
  /** Desired encoder velocity provided in the robot reference order
   * May be empty */
  std::vector<double> encoderVelocities;

  /** Compute required buffer size */
  size_t size() const;
  /** Fill pre-allocated buffer, must be of (at-least) size() */
  void toBuffer(uint8_t * buffer) const;
  /** Fill from a provided buffer */
  void fromBuffer(uint8_t * buffer);
};

} // namespace mc_udp
