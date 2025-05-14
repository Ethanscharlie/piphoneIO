#include "io/display.hpp"
#include "io/globaldata.hpp"
#include <gtest/gtest.h>

int main() {
  PiPIO::init();

  // for (int x = 20; x < 100; x++) {
  //   for (int y = 20; y < 30; y++) {
  //     PiPIO::setPixel(x, y, true);
  //   }
  // }
  //
  PiPIO::drawText(10, 10, "HelloWorld!");

  PiPIO::refreshDisplay();

  while (1) {
    signal(SIGINT, [](int sig) {
      PiPIO::end();
      std::exit(1);
    });
  }
}
