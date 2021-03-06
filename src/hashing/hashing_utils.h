// Copyright (c) 2016 Alexander Gallego. All rights reserved.
//
#pragma once
#include <boost/crc.hpp>
#include <xxhash.h>

#include "platform/log.h"
#include "platform/macros.h"

namespace smf {
inline uint32_t
crc_32(const char *data, const size_t &length) {
  boost::crc_32_type result;
  result.process_bytes(data, length);
  return result.checksum();
}
inline uint64_t
xxhash_64(const char *data, const size_t &length) {
  return XXH64(data, length, 0);
}
inline uint32_t
xxhash_32(const char *data, const size_t &length) {
  return XXH32(data, length, 0);
}

/// google's jump consistent hash
/// https://arxiv.org/pdf/1406.2294.pdf
// TODO(agallego) - move this to it's own impl & header
__attribute__((unused)) static uint32_t
jump_consistent_hash(uint64_t key, uint32_t num_buckets) {
  int64_t b = -1, j = 0;
  while (j < num_buckets) {
    b   = j;
    key = key * 2862933555777941757ULL + 1;
    j   = (b + 1) *
        (static_cast<double>(1LL << 31) / static_cast<double>((key >> 33) + 1));
  }
  return static_cast<uint32_t>(b);
}

class incremental_xxhash64 {
 public:
  SMF_DISALLOW_COPY_AND_ASSIGN(incremental_xxhash64);

  incremental_xxhash64() { reset(); }
  [[gnu::always_inline]] inline void
  reset() {
    // no need to check for error
    // https://gist.github.com/9ea1c9ad4df3bad8b16e4dea4a18018a
    XXH64_reset(&state_, 0);
  }
  [[gnu::always_inline]] inline void
  update(const char *src, const std::size_t &sz) {
    XXH64_update(&state_, src, sz);
  }
  template <typename T,
            class = typename std::enable_if<std::is_integral<T>::value>::type>
  [[gnu::always_inline]] inline void
  update(T t) {
    XXH64_update(&state_, (const char *)&t, sizeof(T));
  }
  [[gnu::always_inline]] inline uint64_t
  digest() {
    return XXH64_digest(&state_);
  }

 private:
  XXH64_state_t state_;
};

class incremental_xxhash32 {
 public:
  SMF_DISALLOW_COPY_AND_ASSIGN(incremental_xxhash32);

  incremental_xxhash32() { reset(); }
  [[gnu::always_inline]] inline void
  reset() {
    // no need to check for error
    // https://gist.github.com/9ea1c9ad4df3bad8b16e4dea4a18018a
    XXH32_reset(&state_, 0);
  }
  [[gnu::always_inline]] inline void
  update(const char *src, const std::size_t &sz) {
    XXH32_update(&state_, src, sz);
  }
  template <typename T,
            class = typename std::enable_if<std::is_integral<T>::value>::type>
  [[gnu::always_inline]] inline void
  update(T t) {
    XXH32_update(&state_, (const char *)&t, sizeof(T));
  }
  [[gnu::always_inline]] inline uint32_t
  digest() {
    return XXH32_digest(&state_);
  }

 private:
  XXH32_state_t state_;
};

}  // namespace smf
