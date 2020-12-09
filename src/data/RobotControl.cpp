/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#include <mc_udp/data/RobotControl.h>
#include <mc_udp/data/utils.h>

#include <cstring>

namespace mc_udp
{

size_t RobotControl::size() const
{
  return sizeof(uint64_t) + sizeof(uint64_t) + encoders.size() * sizeof(double) + sizeof(uint64_t)
         + encoderVelocities.size() * sizeof(double);
}

size_t RobotControl::toBuffer(uint8_t * buffer) const
{
  size_t offset = 0;
  utils::memcpy_advance(buffer, &id, sizeof(uint64_t), offset);
  utils::toBuffer(buffer, encoders, offset);
  utils::toBuffer(buffer, encoderVelocities, offset);
  return offset;
}

size_t RobotControl::fromBuffer(uint8_t * buffer)
{
  size_t offset = 0;
  utils::memcpy_advance(&id, buffer, sizeof(uint64_t), offset);
  utils::fromBuffer(encoders, buffer, offset);
  utils::fromBuffer(encoderVelocities, buffer, offset);
  return offset;
}

} // namespace mc_udp
