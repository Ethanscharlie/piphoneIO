#pragma once

#include <cmath>
#include <string>
namespace PiPIO {

inline const int CHARPX_WIDTH = 4;
inline const int CHARPX_HEIGHT = 8;
inline const int CHAR_PAD = 1;
inline const int DISPLAY_WIDTH = 128;
inline const int DISPLAY_HEIGHT = 64;

extern const int MAX_CHAR_HOR;
extern const int MAX_CHAR_VER;

void init();
void end();
void refreshDisplay();
void clearDisplay();
void setPixel(int x, int y, bool value);
void drawText(int x, int y, std::string text);

} // namespace PiPIO
