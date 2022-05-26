#ifndef GCRYPTEXAMPLE_BENCHMARK_H
#define GCRYPTEXAMPLE_BENCHMARK_H

#include <functional>
#include <chrono>
#include <iostream>

void Benchmark(const std::string& brief, std::function<void()> toBenchmark) {
  std::cout << "Benchmarking " << brief << "..." << std::endl;

  auto start = std::chrono::steady_clock::now();

  toBenchmark();

  auto end = std::chrono::steady_clock::now();
  double seconds = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
  std::cout << seconds << " seconds." << std::endl << std::endl;

  return;
}

#endif

