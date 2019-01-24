#include <mc_nng/data/RobotSensors.h>

#include <cstring>

namespace mc_nng
{

void RobotSensors::fsensor(const std::string & name, double data[6])
{
  for(auto & fs : fsensors)
  {
    if(fs.name == name)
    {
      std::memcpy(fs.reading, data, 6 * sizeof(double));
      return;
    }
  }
  fsensors.emplace_back();
  fsensors.back().name = name;
  std::memcpy(fsensors.back().reading, data, 6 * sizeof(double));
}

size_t RobotSensors::size() const
{
  return
    // Size of id
    sizeof(uint64_t) +
    // Size of encoders buffer lenght + data
    sizeof(uint64_t) + encoders.size() * sizeof(double) +
    // Size of torques buffer lenght + data
    sizeof(uint64_t) + torques.size() * sizeof(double)  +
    // Size of fsensors
    fsensorsSize() +
    // Size of orientation + angularVelocity + angularAcceleration
    9 * sizeof(double);
}

size_t RobotSensors::fsensorsSize() const
{
  size_t ret = sizeof(uint64_t); // Lenght of fSensors
  for(const auto & fsensor : fsensors)
  {
    ret += sizeof(uint64_t) + fsensor.name.size() * sizeof(char) + 6 * sizeof(double);
  }
  return ret;
}

void RobotSensors::toBuffer(uint8_t * buffer) const
{
  size_t offset = 0;
  uint64_t tmp = 0; // temporary to store size
  static_assert(sizeof(decltype(encoders)::size_type) == sizeof(uint64_t), "Expect vector<double>::size() to return an unsigned integer of 64 bits");
  static_assert(sizeof(std::string::size_type) == sizeof(uint64_t), "Expect std::string::size() to return an unsigned integer of 64 bits");

  // memcpy and advance the offset
  auto memcpy_advance = [&offset](uint8_t * dest, const void * src, size_t n)
  {
    std::memcpy(dest + offset, src, n);
    offset += n;
  };

  memcpy_advance(buffer, &id, sizeof(uint64_t));

  tmp = encoders.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t));
  memcpy_advance(buffer, encoders.data(), encoders.size() * sizeof(double));

  tmp = torques.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t));
  memcpy_advance(buffer, torques.data(), torques.size() * sizeof(double));

  tmp = fsensors.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t));
  for(auto & fs : fsensors)
  {
    tmp = fs.name.size();
    memcpy_advance(buffer, &tmp, sizeof(uint64_t));
    memcpy_advance(buffer, fs.name.c_str(), fs.name.size() * sizeof(char));
    memcpy_advance(buffer, fs.reading, 6 * sizeof(double));
  }
  memcpy_advance(buffer, orientation, 3 * sizeof(double));
  memcpy_advance(buffer, angularVelocity, 3 * sizeof(double));
  memcpy_advance(buffer, angularAcceleration, 3 * sizeof(double));
}

void RobotSensors::fromBuffer(uint8_t * buffer)
{
  size_t offset = 0;
  uint64_t tmp = 0; // temporary to store size

  // memcpy and advance the offset
  auto memcpy_advance = [&offset](void * dest, const uint8_t * src, size_t n)
  {
    std::memcpy(dest, src + offset, n);
    offset += n;
  };

  memcpy_advance(&id, buffer, sizeof(uint64_t));

  memcpy_advance(&tmp, buffer, sizeof(uint64_t));
  encoders.resize(tmp);
  memcpy_advance(encoders.data(), buffer, tmp * sizeof(double));

  memcpy_advance(&tmp, buffer, sizeof(uint64_t));
  torques.resize(tmp);
  memcpy_advance(torques.data(), buffer, tmp * sizeof(double));

  memcpy_advance(&tmp, buffer, sizeof(uint64_t));
  fsensors.resize(tmp);
  for(auto & fs : fsensors)
  {
    memcpy_advance(&tmp, buffer, sizeof(uint64_t));
    fs.name.assign(reinterpret_cast<char *>(&buffer[offset]), tmp * sizeof(char));
    offset += tmp * sizeof(char);
    memcpy_advance(fs.reading, buffer, 6 * sizeof(double));
  }
  memcpy_advance(orientation, buffer, 3 * sizeof(double));
  memcpy_advance(angularVelocity, buffer, 3 * sizeof(double));
  memcpy_advance(angularAcceleration, buffer, 3 * sizeof(double));
}

}
