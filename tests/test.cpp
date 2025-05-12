#include "io/display.hpp"
#include "io/globaldata.hpp"
#include <SDL_render.h>
#include <gtest/gtest.h>

int main() {
  PiPIO::init();

  SDL_Rect r = {0, 0, 30, 100};
  SDL_RenderFillRect(PiPIO::renderer, &r);

  PiPIO::refreshDisplay();

  while (1) {
    signal(SIGINT, [](int sig) { std::exit(1); });
  }
}
