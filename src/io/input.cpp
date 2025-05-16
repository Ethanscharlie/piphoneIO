#include "io/io.hpp"

namespace PiPIO {

Buttons getButtons() {
  Buttons buttons;

#ifdef SIM
#else
  buttons.a = (bcm2835_gpio_lev(PiPIO::A_BUTTON_PIN) == HIGH) ? 1 : 0;
  buttons.b = (bcm2835_gpio_lev(PiPIO::B_BUTTON_PIN) == HIGH) ? 1 : 0;
  buttons.c = (bcm2835_gpio_lev(PiPIO::C_BUTTON_PIN) == HIGH) ? 1 : 0;
  buttons.d = (bcm2835_gpio_lev(PiPIO::D_BUTTON_PIN) == HIGH) ? 1 : 0;
#endif

  return buttons;
}

} // namespace PiPIO
