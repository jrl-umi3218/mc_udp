#pragma once

#include <mc_udp/data/MultiRobotMessage.h>

using RobotSensors = mc_udp::RobotSensors;

inline RobotSensors & getSensors(std::unordered_map<std::string, RobotSensors> * m, const std::string & k)
{
  return (*m)[k];
}

using RobotControl = mc_udp::RobotControl;

inline RobotControl & getControl(std::unordered_map<std::string, RobotControl> * m, const std::string & k)
{
  return (*m)[k];
}

template<typename U>
std::vector<std::string> getKeys(const std::unordered_map<std::string, U> * m)
{
  std::vector<std::string> ret;
  ret.reserve(m->size());
  for(const auto & e : *m)
  {
    ret.push_back(e.first);
  }
  return ret;
}
