#pragma once

#include <cmath>
namespace PiPIO {

inline const int CHARPX_WIDTH = 5;
inline const int CHARPX_HEIGHT = 8;
inline const int CHAR_PAD = 1;
inline const int DISPLAY_WIDTH = 128;
inline const int DISPLAY_HEIGHT = 64;

extern const int MAX_CHAR_HOR;
extern const int MAX_CHAR_VER;

void init();
void refreshDisplay();
void setPixel(int x, int y, bool value);

} // namespace PiPIO
