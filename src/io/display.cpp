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

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void clearDisplay() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

void setPixel(int x, int y, bool value) {
  if (value) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  }

  SDL_RenderDrawPoint(renderer, x, y);
}

void drawText(int x, int y, std::string text) {}

void refreshDisplay() {
  // DEMO DISPLAY DRAW
  SDL_RenderPresent(renderer);
}

} // namespace PiPIO
