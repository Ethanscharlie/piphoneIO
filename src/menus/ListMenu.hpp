#pragma once

#include "io/io.hpp"
#include "menus/Menu.hpp"
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

class ListMenu : public Menu {
public:
  std::vector<std::pair<std::string, std::function<void()>>> options;
  int selection = 0;

  ListMenu(std::vector<std::pair<std::string, std::function<void()>>> options)
      : options(options) {}

  void render() override {
    if (options.empty()) {
      throw std::logic_error("Menu needs at least one option");
    }

    int rangeMin = 0;
    int rangeMax = options.size();

    if ((options.size() + PiPIO::CHAR_PAD) * PiPIO::CHARPX_HEIGHT >
        PiPIO::DISPLAY_HEIGHT - 15) {
      rangeMin = selection;
      // TODO rangeMax =
    }
    for (int i = rangeMin; i < rangeMax; i++) {
      std::string text = "";

      if (i == selection) {
        text += ">";
      } else {
        text += " ";
      }

      text += options[i].first;

      int iPos = i - rangeMin;
      int y = 16 + iPos * PiPIO::CHARPX_HEIGHT + PiPIO::CHAR_PAD;
      PiPIO::drawText(0, y, text);
    }
  }

  void onAButton() override { options[selection].second(); }

  void onJoystick(int x, int y) override {
    if (y == -1) {
      selection++;
      if (selection == options.size()) {
        selection = 0;
      }
    }

    else if (y == 1) {
      selection--;
      if (selection < 0) {
        selection = options.size() - 1;
      }
    }
  }
};
