#include "io/display.hpp"
#include "io/globaldata.hpp"
#include "menus/HomeMenu.hpp"
#include <gtest/gtest.h>

int main() {
  PiPIO::init();
  PiPIO::refreshDisplay();

  HomeMenu homeMenu = HomeMenu({"Music", "Settings", "Audobooks"});

  Menu *currentMenu = &homeMenu;
  currentMenu->render();
  PiPIO::refreshDisplay();

  while (1) {
#ifdef SIM

#else

#endif // SIM
    signal(SIGINT, [](int sig) {
      PiPIO::end();
      std::exit(1);
    });
  }
}
