
#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "io/io.hpp"
#include "menus/Menu.hpp"

class StopwatchMenu : public Menu {
  int selection = 0;

  bool running = false;
  float recordedTime = 0;
  float startTime;

  float getCurrentMili() {
    auto now = std::chrono::system_clock::now();
    float milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                             now.time_since_epoch())
                             .count();
    return milliseconds;
  }

  float getElapsedTimeSinceStart() { return getCurrentMili() - startTime; }

  std::string getClockString() {
    float time = recordedTime + getElapsedTimeSinceStart();
    return std::to_string(time);
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
  void render() override {
    std::vector<std::string> options = {"Start", "Plause", "Clear"};
    for (int i = 0; i < options.size(); i++) {
      std::string line = "";

      if (selection == i) {
        line += " ";
      } else {
        line += ">";
      }

      line += options[i];

      int y = 16 + i * PiPIO::CHARPX_HEIGHT + PiPIO::CHAR_PAD;
      PiPIO::drawText(0, y, line);
    }

    int timeY =
        16 + (options.size() + 2) * PiPIO::CHARPX_HEIGHT + PiPIO::CHAR_PAD;

    PiPIO::drawText(0, timeY, getClockString());
  }

  void onInput(bool a, bool b, bool c, bool d) override {}
};
