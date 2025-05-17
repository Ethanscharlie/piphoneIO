#pragma once

#include <chrono>

inline long getCurrentMili() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  long milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return milliseconds;
}
