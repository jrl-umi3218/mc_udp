#pragma once

#include "MultiRobotMessage.h"

namespace mc_udp
{

template<typename MsgT>
size_t MultiRobotMessage<MsgT>::size() const
{
  size_t ret = sizeof(uint64_t); // Number of robots in the message
  for(auto it = messages.cbegin(); it != messages.cend(); it++)
  {
    ret +=
        // Length of robot name
        sizeof(uint64_t) +
        // Name data
        it->first.size() * sizeof(char) +
        // Length of message for that robot
        it->second.size();
  }
  return ret;
}

template<typename MsgT>
size_t MultiRobotMessage<MsgT>::toBuffer(uint8_t * buffer) const
{
  size_t offset = 0;
  uint64_t tmp = messages.size();
  utils::memcpy_advance(buffer, &tmp, sizeof(uint64_t), offset);
  for(auto it = messages.cbegin(); it != messages.cend(); it++)
  {
    utils::toBuffer(buffer, it->first, offset);
    offset += it->second.toBuffer(buffer + offset);
  }
  return offset;
}

template<typename MsgT>
size_t MultiRobotMessage<MsgT>::fromBuffer(uint8_t * buffer)
{
  size_t offset = 0;
  uint64_t nRobots = 0;
  utils::memcpy_advance(&nRobots, buffer, sizeof(uint64_t), offset);
  for(uint64_t i = 0; i < nRobots; ++i)
  {
    std::string name;
    utils::fromBuffer(name, buffer, offset);
    offset += messages[name].fromBuffer(buffer + offset);
  }
  return offset;
}

} // namespace mc_udp
