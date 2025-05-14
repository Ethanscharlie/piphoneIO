#pragma once

#include "io/display.hpp"
#include "menus/Menu.hpp"
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

class ListMenu : public Menu {
  std::vector<std::pair<std::string, std::function<void()>>> options;
  int selection = 0;

public:
  ListMenu(std::vector<std::pair<std::string, std::function<void()>>> options)
      : options(options) {
    if (options.empty()) {
      throw std::logic_error("Home Menu needs at least one option");
    }
  }

  void render() override {
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

  void onInput(bool a, bool b, bool c, bool d) override {
    if (!a && !b && !c && d) {
      selection++;
      if (selection == options.size()) {
        selection = 0;
      }
    }

    if (a && !b && !c && !d) {
      selection--;
      if (selection < 0) {
        selection = options.size() - 1;
      }
    }

    if (!a && !b && c && !d) {
      options[selection].second();
    }
  }
};
