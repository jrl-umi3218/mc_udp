/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#include <mc_udp/data/RobotControl.h>
#include <mc_udp/data/RobotSensors.h>
#include <mc_udp/data/utils.h>

#include <unordered_map>

namespace mc_udp
{

/** Serialize/Deserialize a message for multiple robots */
template<typename MsgT>
struct MultiRobotMessage
{
  /** Map robot identifier (name) to message */
  std::unordered_map<std::string, MsgT> messages;

  /** Compute required buffer size */
  size_t size() const;
  /** Fill pre-allocated buffer, must be of (at-least) size()
   *
   * Returns the number of bytes written */
  size_t toBuffer(uint8_t * buffer) const;
  /** Fill from a provided buffer
   *
   * Returns the number of bytes consumed */
  size_t fromBuffer(uint8_t * buffer);
};

using MultiRobotControl = MultiRobotMessage<RobotControl>;
using MultiRobotSensors = MultiRobotMessage<RobotSensors>;

} // namespace mc_udp

#include "MultiRobotMessage.hpp"
