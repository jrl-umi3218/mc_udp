#include <mc_udp/data/RobotSensors.h>

#include <cstring>

namespace mc_udp
{

void RobotSensors::fsensor(const std::string & name, double data[6])
{
  for(size_t i = 0; i < fsensors.size(); ++i)
  {
    auto & fs = fsensors[i];
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
      sizeof(uint64_t) + torques.size() * sizeof(double) +
      // Size of fsensors
      fsensorsSize() +
      // Size of orientation + angularVelocity + angularAcceleration
      9 * sizeof(double);
}

size_t RobotSensors::fsensorsSize() const
{
  size_t ret = sizeof(uint64_t); // Lenght of fSensors
  for(size_t i = 0; i < fsensors.size(); ++i)
  {
    const auto & fsensor = fsensors[i];
    ret += sizeof(uint64_t) + fsensor.name.size() * sizeof(char) + 6 * sizeof(double);
  }
  return ret;
}

namespace
{

void memcpy_advance(uint8_t * dest, const void * src, size_t n, size_t & offset)
{
  std::memcpy(dest + offset, src, n);
  offset += n;
}

} // namespace

void RobotSensors::toBuffer(uint8_t * buffer) const
{
  size_t offset = 0;
  uint64_t tmp = 0; // temporary to store size

  memcpy_advance(buffer, &id, sizeof(uint64_t), offset);

  tmp = encoders.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t), offset);
  memcpy_advance(buffer, encoders.data(), encoders.size() * sizeof(double), offset);

  tmp = torques.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t), offset);
  memcpy_advance(buffer, torques.data(), torques.size() * sizeof(double), offset);

  tmp = fsensors.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t), offset);
  for(size_t i = 0; i < fsensors.size(); ++i)
  {
    auto & fs = fsensors[i];
    tmp = fs.name.size();
    memcpy_advance(buffer, &tmp, sizeof(uint64_t), offset);
    memcpy_advance(buffer, fs.name.c_str(), fs.name.size() * sizeof(char), offset);
    memcpy_advance(buffer, fs.reading, 6 * sizeof(double), offset);
  }
  memcpy_advance(buffer, orientation, 3 * sizeof(double), offset);
  memcpy_advance(buffer, angularVelocity, 3 * sizeof(double), offset);
  memcpy_advance(buffer, angularAcceleration, 3 * sizeof(double), offset);
}

namespace
{

void memcpy_advance(void * dest, const uint8_t * src, size_t n, size_t & offset)
{
  std::memcpy(dest, src + offset, n);
  offset += n;
}

} // namespace

void RobotSensors::fromBuffer(uint8_t * buffer)
{
  size_t offset = 0;
  uint64_t tmp = 0; // temporary to store size

  memcpy_advance(&id, buffer, sizeof(uint64_t), offset);

  memcpy_advance(&tmp, buffer, sizeof(uint64_t), offset);
  encoders.resize(tmp);
  memcpy_advance(encoders.data(), buffer, tmp * sizeof(double), offset);

  memcpy_advance(&tmp, buffer, sizeof(uint64_t), offset);
  torques.resize(tmp);
  memcpy_advance(torques.data(), buffer, tmp * sizeof(double), offset);

  memcpy_advance(&tmp, buffer, sizeof(uint64_t), offset);
  fsensors.resize(tmp);
  for(size_t i = 0; i < fsensors.size(); ++i)
  {
    auto & fs = fsensors[i];
    tmp = fs.name.size();
    memcpy_advance(&tmp, buffer, sizeof(uint64_t), offset);
    fs.name.assign(reinterpret_cast<char *>(&buffer[offset]), tmp * sizeof(char));
    offset += tmp * sizeof(char);
    memcpy_advance(fs.reading, buffer, 6 * sizeof(double), offset);
  }
  memcpy_advance(orientation, buffer, 3 * sizeof(double), offset);
  memcpy_advance(angularVelocity, buffer, 3 * sizeof(double), offset);
  memcpy_advance(angularAcceleration, buffer, 3 * sizeof(double), offset);
}

} // namespace mc_udp
