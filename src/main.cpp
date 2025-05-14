#include "io/display.hpp"
#include "io/globaldata.hpp"
#include "menus/HomeMenu.hpp"
#include "menus/ListMenu.hpp"
#include <gtest/gtest.h>

#ifdef SIM
#include <SDL_events.h>
#include <SDL_keycode.h>
#else
#include <bcm2835.h>
#endif

int main() {
#ifdef SIM
#else
  bool prevA = 0;
  bool prevB = 0;
  bool prevC = 0;
  bool prevD = 0;
#endif

  PiPIO::init();
  PiPIO::refreshDisplay();

  Menu *currentMenu = nullptr;

  ListMenu musicMenu =
      ListMenu({"Play", "Pause", "Queue All", "Vol Up", "Vol Down", "Other",
                "Another", "Pizza", "Ice Cream"});

  HomeMenu homeMenu = HomeMenu({{"Music", [&]() { currentMenu = &musicMenu; }},
                                {"Settings", []() {}},
                                {"Audobooks", []() {}}});

  currentMenu = &homeMenu;
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

        if (event.key.keysym.sym == SDLK_q) {
          PiPIO::end();
          std::exit(1);
        }

        currentMenu->render();
        PiPIO::refreshDisplay();
      }
    }
#else
    bool a = (bcm2835_gpio_lev(PiPIO::A_BUTTON_PIN) == HIGH) ? 1 : 0;
    bool b = (bcm2835_gpio_lev(PiPIO::B_BUTTON_PIN) == HIGH) ? 1 : 0;
    bool c = (bcm2835_gpio_lev(PiPIO::C_BUTTON_PIN) == HIGH) ? 1 : 0;
    bool d = (bcm2835_gpio_lev(PiPIO::D_BUTTON_PIN) == HIGH) ? 1 : 0;

    bool buttonReleased = false;

    if (prevA && !a) {
      PiPIO::clearDisplay();
      currentMenu->onInput(1, 0, 0, 0);
      buttonReleased = true;
    }

    if (prevD && !d) {
      PiPIO::clearDisplay();
      currentMenu->onInput(0, 0, 0, 1);
      buttonReleased = true;
    }

    prevA = a;
    prevB = b;
    prevC = c;
    prevD = d;

    if (buttonReleased) {
      currentMenu->render();
      PiPIO::refreshDisplay();
    }

#endif // SIM
    signal(SIGINT, [](int sig) {
      PiPIO::end();
      std::exit(1);
    });
  }
}
