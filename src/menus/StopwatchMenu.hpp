
#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "io/io.hpp"
#include "menus/Menu.hpp"

class StopwatchMenu : public Menu {
  int selection = 0;

  bool running = false;
  long recordedTime = 0;
  long startTime = 0;

  long getCurrentMili() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    long milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return milliseconds;
  }

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
  void render() override { PiPIO::drawText(20, 20, getClockString()); }

  void onInput(bool a, bool b, bool c, bool d) override {
    if (a && !b && !c && !d) {
      startClock();
    }

    if (!a && !b && c && !d) {
      pauseClock();
    }

    if (!a && !b && !c && d) {
      clearClock();
    }
  }
};
