#pragma once

#include <cmath>
#include <string>

#ifdef SIM

#else
#include <bcm2835.h>
#endif // SIM

namespace PiPIO {

#ifdef SIM

#else
// inline const int A_BUTTON_PIN = RPI_GPIO_P1_07;
// inline const int B_BUTTON_PIN = RPI_GPIO_P1_16;
// inline const int C_BUTTON_PIN = RPI_GPIO_P1_12;
// inline const int D_BUTTON_PIN = RPI_GPIO_P1_21;

inline const int A_BUTTON_PIN = RPI_GPIO_P1_12;
inline const int B_BUTTON_PIN = RPI_GPIO_P1_16;
inline const int C_BUTTON_PIN = RPI_GPIO_P1_18;
inline const int D_BUTTON_PIN = RPI_GPIO_P1_22;
#endif // SIM

inline const int CHARPX_WIDTH = 5;
inline const int CHARPX_HEIGHT = 8;
inline const int CHAR_PAD = 1;
inline const int DISPLAY_WIDTH = 128;
inline const int DISPLAY_HEIGHT = 64;

extern const int MAX_CHAR_HOR;
extern const int MAX_CHAR_VER;

void runSystemCommand(std::string command);
void init();
void end();
void refreshDisplay();
void clearDisplay();
void setPixel(int x, int y, bool value);
void drawText(int x, int y, std::string text);

} // namespace PiPIO
