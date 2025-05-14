#pragma once

#include "io/display.hpp"
#include "menus/Menu.hpp"
#include <stdexcept>
#include <string>
#include <vector>

class HomeMenu : public Menu {
  std::vector<std::string> options;
  int selection = 0;

public:
  HomeMenu(std::vector<std::string> options) : options(options) {
    if (options.empty()) {
      throw std::logic_error("Home Menu needs at least one option");
    }
  }

  void render() override {
    std::string headerText = "";
    for (int i = 0; i < options.size(); i++) {
      if (i == selection) {
        headerText += " o";
      } else {
        headerText += " .";
      }
    }
    PiPIO::drawText(0, 0, headerText);

    PiPIO::drawText(5, 16 + 5, options[selection]);
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
  }
};
