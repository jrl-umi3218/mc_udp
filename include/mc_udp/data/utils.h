/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#include <cstring>
#include <string>
#include <vector>

namespace mc_udp
{

namespace utils
{

/** Copy n bytes from src to dest + offset and advance offset by n */
inline void memcpy_advance(uint8_t * dest, const void * src, size_t n, size_t & offset)
{
  std::memcpy(dest + offset, src, n);
  offset += n;
}

/** Copy n bytes from src + offset to dest and advance offset by n */
inline void memcpy_advance(void * dest, const uint8_t * src, size_t n, size_t & offset)
{
  std::memcpy(dest, src + offset, n);
  offset += n;
}

/** Write src to dest + offset and correctly advance the offset
 *
 * T must have the following methods and member types:
 * - T::value_type (type of values in the container)
 * - T::size() (returns the size of the container)
 * - T::data() (returns a pointer to the contiguous slice of memory containing the data
 *
 * In practice this is used with std::string and std::vector<double> in this code
 *
 */
template<typename T>
void toBuffer(uint8_t * dest, const T & src, size_t & offset)
{
  uint64_t tmp = src.size();
  memcpy_advance(dest, &tmp, sizeof(uint64_t), offset);
  memcpy_advance(dest, src.data(), tmp * sizeof(typename T::value_type), offset);
}

/** Read dest from src + offset and correctly advance the offset
 *
 * T must have the following methods and member types:
 * - T::value_type (type of values in the container)
 * - T::resize() (returns the size of the container)
 * - T::data() (returns a pointer to the contiguous slice of memory containing the data
 *
 * In practice this is used with std::string and std::vector<double> in this code
 *
 */
template<typename T>
void fromBuffer(T & dest, const uint8_t * src, size_t & offset)
{
  uint64_t tmp = 0;
  memcpy_advance(&tmp, src, sizeof(uint64_t), offset);
  dest.resize(tmp);
  memcpy_advance(const_cast<typename T::value_type *>(dest.data()), src, tmp * sizeof(typename T::value_type), offset);
}

} // namespace utils

} // namespace mc_udp
