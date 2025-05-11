#include "piphone.hpp"
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
typedef std::vector<std::vector<bool>> Charmap;

static Charmap drawChar(char c, int x, int y) {
  switch (c) {
  case 'a':
    return {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {1, 0, 0, 1, 0},
            {1, 1, 1, 1, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {0, 0, 0, 0, 0}};
  case 'b':
    return {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 0, 0}, {1, 0, 0, 1, 0},
            {1, 1, 1, 0, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 0, 0}, {0, 0, 0, 0, 0}};
  default:
    return {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
  }
}

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
}

void refreshDisplay() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for (int y = CHAR_PAD; y < DISPLAY_HEIGHT; y += CHAR_PAD + CHARPX_HEIGHT) {
    for (int x = CHAR_PAD; x < DISPLAY_WIDTH; x += CHAR_PAD + CHARPX_WIDTH) {
      char c = 'a';

      if (x > y) {
        c = 'b';
      }

      Charmap charmap = drawChar(c, x, y);

      for (int chary = 0; chary < charmap.size(); chary++) {
        for (int charx = 0; charx < charmap[0].size(); charx++) {
          if (charmap[chary][charx]) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          }
          SDL_RenderDrawPoint(renderer, x + charx, y + chary);
        }
      }
    }
  }

  SDL_RenderPresent(renderer);
}

} // namespace PiPIO
