#ifdef SIM
#include <SDL2/SDL.h>
#else

#endif
#include <string>

namespace PiPIO {
#ifdef SIM
inline SDL_Renderer *renderer = nullptr;
inline SDL_Window *window = nullptr;

inline SDL_Texture *screenTexture = nullptr;
#else

#endif
} // namespace PiPIO
