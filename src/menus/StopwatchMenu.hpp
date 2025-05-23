
#pragma once

#include <format>
#include <string>
#include <unistd.h>

#include "io/io.hpp"
#include "menus/Menu.hpp"
#include "utils.hpp"

class StopwatchMenu {

  bool running = false;
  long recordedTime = 0;
  long startTime = 0;

  long getElapsedTimeSinceStart() { return getCurrentMili() - startTime; }

  std::string getClockString() {
    long time;

    if (running) {
      time = recordedTime + getElapsedTimeSinceStart();
    } else {
      time = recordedTime;
    }

    unsigned long seconds = time / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    unsigned long days = hours / 24;
    time %= 1000;
    seconds %= 60;
    minutes %= 60;
    hours %= 24;

    return std::format("{}:{}:{}", hours, minutes, seconds);
  }

  void startClock() {
    if (!running) {
      running = true;
      startTime = getCurrentMili();
    }
  }

  void pauseClock() {
    if (running) {
      recordedTime = getElapsedTimeSinceStart();
      running = false;
    }
  }

  void clearClock() {
    recordedTime = 0;
    running = false;
  }

public:
  Menu menu;
};
