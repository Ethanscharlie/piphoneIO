#include "piphone.hpp"
#include <gtest/gtest.h>

int main() {
  PiPIO::init();
  PiPIO::refreshDisplay();

  while (1) {
    signal(SIGINT, [](int sig) { std::exit(1); });
  }
}
