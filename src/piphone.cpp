#include "piphone.hpp"
#include "globaldata.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL_render.h>
#include <iostream>
#include <stdexcept>

namespace PiPIO {
const int MAX_CHAR_HOR =
    std::floor((float)DISPLAY_WIDTH / (float)(CHARPX_WIDTH + CHAR_PAD * 2));
const int MAX_CHAR_VER =
    std::floor((float)DISPLAY_HEIGHT / (float)(CHARPX_HEIGHT + CHAR_PAD * 2));
} // namespace PiPIO

void PiPIO::init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::logic_error(SDL_GetError());
  }

  window = SDL_CreateWindow("Piphone Simulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 128, 64, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  std::cout << "Max char hor: " << PiPIO::MAX_CHAR_HOR << "\n";
  std::cout << "Max char ver: " << PiPIO::MAX_CHAR_VER << "\n";
}

void PiPIO::refreshDisplay() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}
