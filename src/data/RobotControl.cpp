#include <mc_nng/data/RobotControl.h>

#include <cstring>

namespace mc_nng
{

size_t RobotControl::size() const
{
  return sizeof(uint64_t) +
         sizeof(uint64_t) + encoders.size() * sizeof(double);
}

namespace
{

void memcpy_advance(uint8_t * dest, const void * src, size_t n, size_t & offset)
{
  std::memcpy(dest + offset, src, n);
  offset += n;
}

}

void RobotControl::toBuffer(uint8_t * buffer) const
{
  size_t offset = 0;
  size_t tmp = 0;

  memcpy_advance(buffer, &id, sizeof(uint64_t), offset);

  tmp = encoders.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t), offset);
  memcpy_advance(buffer, encoders.data(), tmp * sizeof(double), offset);
}

namespace
{

void memcpy_advance(void * dest, const uint8_t * src, size_t n, size_t & offset)
{
  std::memcpy(dest, src + offset, n);
  offset += n;
}

}

void RobotControl::fromBuffer(uint8_t * buffer)
{
  size_t offset = 0;
  uint64_t tmp = 0; // temporary to store size

  memcpy_advance(&id, buffer, sizeof(uint64_t), offset);

  memcpy_advance(&tmp, buffer, sizeof(uint64_t), offset);
  encoders.resize(tmp);
  memcpy_advance(encoders.data(), buffer, tmp * sizeof(double), offset);
}

}
