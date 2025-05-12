#include "display.hpp"
#include "globaldata.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace PiPIO {
const int MAX_CHAR_HOR =
    std::floor((float)DISPLAY_WIDTH / (float)(CHARPX_WIDTH + CHAR_PAD * 2));
const int MAX_CHAR_VER =
    std::floor((float)DISPLAY_HEIGHT / (float)(CHARPX_HEIGHT + CHAR_PAD * 2));

void init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::logic_error(SDL_GetError());
  }

  window = SDL_CreateWindow("Piphone Simulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH,
                            DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH,
                                    DISPLAY_HEIGHT);

  SDL_SetRenderTarget(renderer, screenTexture);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void refreshDisplay() {
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
  SDL_SetRenderTarget(renderer, screenTexture);
}

} // namespace PiPIO
