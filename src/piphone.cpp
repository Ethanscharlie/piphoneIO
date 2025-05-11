#include "piphone.hpp"
#include "globaldata.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL_render.h>
#include <iostream>
#include <stdexcept>

void PiPIO::init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::logic_error(SDL_GetError());
  }

  window =
      SDL_CreateWindow("Piphone Simulator", 0, 0, 128, 64, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void PiPIO::refreshDisplay() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}
