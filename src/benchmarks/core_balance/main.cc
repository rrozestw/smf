// Copyright 2018 Alexander Gallego
//

#include <cstring>
#include <memory>
#include <thread>

#include <benchmark/benchmark.h>

#include <fastrange/fastrange.h>
#include "hashing/hashing_utils.h"

using namespace smf;  // NOLINT

static uint32_t
mod_load(uint64_t x, uint32_t n) {
  return fastrange32(x, n);
}

static uint32_t
jump_load(uint64_t x, uint32_t n) {
  return jump_consistent_hash(x, n);
}

static void
BM_mod_load(benchmark::State &state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(
      mod_load(state.range(0), std::thread::hardware_concurrency()));
  }
}
BENCHMARK(BM_mod_load)
  ->Args({1 << 1, 1 << 1})
  ->Args({1 << 4, 1 << 4})
  ->Args({1 << 8, 1 << 8})
  ->Args({1 << 16, 1 << 16});

static void
BM_jump_load(benchmark::State &state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(
      jump_load(state.range(0), std::thread::hardware_concurrency()));
  }
}
BENCHMARK(BM_jump_load)
  ->Args({1 << 1, 1 << 1})
  ->Args({1 << 4, 1 << 4})
  ->Args({1 << 8, 1 << 8})
  ->Args({1 << 16, 1 << 16});


BENCHMARK_MAIN();
