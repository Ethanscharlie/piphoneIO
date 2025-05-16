#pragma once

#include "io/io.hpp"
#include "menus/Menu.hpp"
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class HomeMenu : public Menu {
  std::vector<std::pair<std::string, std::function<void()>>> options;
  int selection = 0;

public:
  HomeMenu(std::vector<std::pair<std::string, std::function<void()>>> options)
      : options(options) {
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

    int padding = (PiPIO::MAX_CHAR_HOR - headerText.size()) / 2;

    for (int i = 0; i < padding; i++) {
      headerText = " " + headerText;
    }

    PiPIO::drawText(0, 0, headerText);

    PiPIO::drawText(5, 16 + 5, options[selection].first);
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
