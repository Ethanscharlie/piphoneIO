#include "Menu.hpp"
#include "io/io.hpp"
#include <stdexcept>

void Menu::render() {
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
    int y = iPos * PiPIO::CHARPX_HEIGHT + PiPIO::CHAR_PAD;
    PiPIO::drawText(0, y, text);
  }
}

void Menu::onAButton() { options[selection].second(); }

void Menu::onJoystick(int x, int y) {
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
