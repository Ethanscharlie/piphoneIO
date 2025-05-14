#include "display.hpp"
#include "globaldata.hpp"
#include <stdexcept>

#ifdef SIM
#include <SDL2/SDL.h>
#else
#include "SSD1306_OLED.hpp"
#include <bcm2835.h>
#endif

namespace PiPIO {
SSD1306 myOLED(DISPLAY_WIDTH, DISPLAY_HEIGHT);

const int MAX_CHAR_HOR =
    std::floor((float)DISPLAY_WIDTH / (float)(CHARPX_WIDTH + CHAR_PAD * 2));
const int MAX_CHAR_VER =
    std::floor((float)DISPLAY_HEIGHT / (float)(CHARPX_HEIGHT + CHAR_PAD * 2));

void init() {
#ifdef SIM
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::logic_error(SDL_GetError());
  }

  window = SDL_CreateWindow("Piphone Simulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH,
                            DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

#else
  const uint16_t I2C_Speed = BCM2835_I2C_CLOCK_DIVIDER_626;
  const uint8_t I2C_Address = 0x3C;
  bool I2C_debug = false;
  printf("OLED Test Begin\r\n");

  // Check if Bcm28235 lib installed and print version.
  if (!bcm2835_init()) {
    throw std::logic_error("Error 1201: init bcm2835 library , Is it installed ?\r\n");
  }

  // Turn on I2C bus (optionally it may already be on)
  if (!myOLED.OLED_I2C_ON()) {
    throw std::logic_error(
        "Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?\n");
    bcm2835_close(); // Close the library
  }

  printf("SSD1306 library Version Number :: %u\r\n", myOLED.getLibVerNum());
  printf("bcm2835 library Version Number :: %u\r\n", bcm2835_version());
  bcm2835_delay(500);

  myOLED.OLEDbegin(I2C_Speed, I2C_Address, I2C_debug); // initialize the OLED
  myOLED.OLEDFillScreen(0xF0,
                        0); // splash screen bars, optional just for effect
  bcm2835_delay(1000);
#endif // SUM
}

void end() {
#ifdef SIM
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
#else
  myOLED.OLEDPowerDown(); // Switch off display
  myOLED.OLED_I2C_OFF();  // Switch off I2C
  bcm2835_close();        // Close the library
#endif // SUM
}

void clearDisplay() {
#ifdef SIM
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
#else

#endif
}

void setPixel(int x, int y, bool value) {
#ifdef SIM
  if (value) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  }

  SDL_RenderDrawPoint(renderer, x, y);

#else
#endif // SUM
}

void drawText(int x, int y, std::string text) {}

void refreshDisplay() {
#ifdef SIM
  SDL_RenderPresent(renderer);

#else
  uint8_t  screenBuffer[(DISPLAY_WIDTH * (DISPLAY_HEIGHT/8))];
  if (!myOLED.OLEDSetBufferPtr(DISPLAY_WIDTH, DISPLAY_HEIGHT, screenBuffer, sizeof(screenBuffer))) return;

  myOLED.OLEDclearBuffer();
  myOLED.setTextColor(WHITE);
  myOLED.setCursor(10, 10);
  myOLED.print("Hello World.");
  myOLED.OLEDupdate();
  delay(1000);
#endif // SUM
}

} // namespace PiPIO
