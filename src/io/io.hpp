#pragma once

#include <cmath>
#include <map>
#include <string>
#include <vector>

#ifdef SIM
#include <SDL2/SDL.h>
#else
#include <bcm2835.h>
#endif // SIM

namespace PiPIO {

typedef std::vector<int> Charmap;

#ifdef SIM

inline SDL_Renderer *renderer = nullptr;
inline SDL_Window *window = nullptr;
inline SDL_Texture *screenTexture = nullptr;

#else

inline const int A_BUTTON_PIN = RPI_GPIO_P1_12;
inline const int B_BUTTON_PIN = RPI_GPIO_P1_16;
inline const int C_BUTTON_PIN = RPI_GPIO_P1_18;
inline const int D_BUTTON_PIN = RPI_GPIO_P1_22;

#endif // SIM

inline const int CHARPX_WIDTH = 5;
inline const int CHARPX_HEIGHT = 8;
inline const int CHAR_PAD = 1;
inline const int DISPLAY_WIDTH = 128;
inline const int DISPLAY_HEIGHT = 128;

extern const int MAX_CHAR_HOR;
extern const int MAX_CHAR_VER;

extern std::map<char, Charmap> font;

void runSystemCommand(std::string command);
void init();
void end();

void refreshDisplay();
void clearDisplay();
void setPixel(int x, int y, bool value);
void drawText(int x, int y, std::string text);

} // namespace PiPIO
