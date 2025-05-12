#include "io/display.hpp"
#include "io/globaldata.hpp"
#include <SDL_render.h>
#include <gtest/gtest.h>

int main() {
  PiPIO::init();

  PiPIO::setPixel(0, 0, true);
  PiPIO::refreshDisplay();

  while (1) {
    signal(SIGINT, [](int sig) { std::exit(1); });
  }
}
