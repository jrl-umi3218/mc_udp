#pragma once

#include <string>
#include <vector>

namespace mc_udp
{

/** Represent a force sensor reading */
struct ForceSensor
{
  std::string name;
  double reading[6];
};

/** Robot sensors' data provided by the server */
struct RobotSensors
{
  /** Unique identifier for the reading */
  uint64_t id;
  /** Joint encoders provided in the robot reference order */
  std::vector<double> encoders;
  /** Joint torques provided in the robot reference order */
  std::vector<double> torques;
  /** Force sensors readings */
  std::vector<ForceSensor> fsensors;
  /** Orientation sensor reading */
  double orientation[3];
  /** Angular velocity sensor */
  double angularVelocity[3];
  /** Angular acceleration */
  double angularAcceleration[3];

  /** Add a force sensor reading */
  void fsensor(const std::string & name, double data[6]);

  /** Compute required buffer size */
  size_t size() const;
  /** Fill pre-allocated buffer, must be of (at-least) size() */
  void toBuffer(uint8_t * buffer) const;
  /** Fill from a provided buffer */
  void fromBuffer(uint8_t * buffer);
private:
  /** Size of required buffer size for force sensors */
  size_t fsensorsSize() const;
};

}
