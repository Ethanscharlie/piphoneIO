#include "io/display.hpp"
#include "io/globaldata.hpp"
#include "menus/HomeMenu.hpp"
#include <gtest/gtest.h>

#ifdef SIM
#include <SDL_events.h>
#include <SDL_keycode.h>
#else
#include <bcm2835.h>
#endif

int main() {
  PiPIO::init();
  PiPIO::refreshDisplay();

  HomeMenu homeMenu = HomeMenu({"Music", "Settings", "Audobooks"});

  Menu *currentMenu = &homeMenu;
  currentMenu->render();
  PiPIO::refreshDisplay();

  while (1) {
#ifdef SIM
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYUP) {
        PiPIO::clearDisplay();

        if (event.key.keysym.sym == SDLK_j)
          currentMenu->onInput(1, 0, 0, 0);
        if (event.key.keysym.sym == SDLK_k)
          currentMenu->onInput(0, 1, 0, 0);
        if (event.key.keysym.sym == SDLK_l)
          currentMenu->onInput(0, 0, 1, 0);
        if (event.key.keysym.sym == SDLK_SEMICOLON)
          currentMenu->onInput(0, 0, 0, 1);

        currentMenu->render();
        PiPIO::refreshDisplay();
      }
    }
#else
    //uint8_t a = bcm2835_gpio_lev(PiPIO::A_BUTTON_PIN);
    //uint8_t b = bcm2835_gpio_lev(PiPIO::B_BUTTON_PIN);
    //uint8_t c = bcm2835_gpio_lev(PiPIO::C_BUTTON_PIN);
    //uint8_t d = bcm2835_gpio_lev(PiPIO::D_BUTTON_PIN);
    //std::cout << a << ", " << b << ", " << c << ", " << d << "\n";

#endif // SIM
    signal(SIGINT, [](int sig) {
      PiPIO::end();
      std::exit(1);
    });
  }
}
