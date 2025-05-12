#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <string>

namespace PiPIO {
inline SDL_Renderer *renderer = nullptr;
inline SDL_Window *window = nullptr;

inline SDL_Texture *screenTexture = nullptr;
} // namespace PiPIO
