#include <mc_nng/data/RobotControl.h>

#include <cstring>

namespace mc_nng
{

size_t RobotControl::size() const
{
  return sizeof(uint64_t) +
         sizeof(uint64_t) + encoders.size() * sizeof(double);
}

void RobotControl::toBuffer(uint8_t * buffer) const
{
  size_t offset = 0;
  size_t tmp = 0;

  // memcpy and advance the offset
  auto memcpy_advance = [&offset](uint8_t * dest, const void * src, size_t n)
  {
    std::memcpy(dest + offset, src, n);
    offset += n;
  };

  memcpy_advance(buffer, &id, sizeof(uint64_t));

  tmp = encoders.size();
  memcpy_advance(buffer, &tmp, sizeof(uint64_t));
  memcpy_advance(buffer, encoders.data(), tmp * sizeof(double));
}

void RobotControl::fromBuffer(uint8_t * buffer)
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
}

}
