#include "io/io.hpp"
#include "menus/HomeMenu.hpp"
#include "menus/ListMenu.hpp"
#include "menus/StopwatchMenu.hpp"
#include "utils.hpp"
#include <filesystem>
#include <gtest/gtest.h>

#ifdef SIM
#include <SDL_events.h>
#include <SDL_keycode.h>
#else
#include <bcm2835.h>
#endif

int main() {
#ifdef SIM
#else
  PiPIO::Buttons prevButtons;
#endif

  PiPIO::init();
  PiPIO::refreshDisplay();

  Menu *currentMenu = nullptr;

  StopwatchMenu stopwatchMenu;

  ListMenu musicArtists({});
  std::filesystem::path musicDir = "/home/ethanscharlie/Music/";
  if (std::filesystem::exists(musicDir)) {
    for (const auto &entry : std::filesystem::directory_iterator(musicDir)) {
      if (std::filesystem::is_directory(entry)) {
        std::string name = entry.path().filename().string();
        musicArtists.options.push_back({name, [name]() {
                                          PiPIO::runSystemCommand(
                                              "mpc clear; mpc add Music/\"" +
                                              name + "\"");
                                          PiPIO::runSystemCommand("mpc play");
                                        }});
      }
    }
  } else {
    musicArtists.options.push_back({"Nothing Found.", []() {}});
  }

  ListMenu musicMenu =
      ListMenu({{"Toggle", [&]() { PiPIO::runSystemCommand("mpc toggle"); }},
                {"Skip", []() { PiPIO::runSystemCommand("mpc next"); }},
                {"Queue All",
                 []() {
                   PiPIO::runSystemCommand(
                       "mpc clear; mpc add Music; mpc shuffle; mpc play");
                 }},
                {"Pick Artist", [&]() { currentMenu = &musicArtists; }}});

  HomeMenu homeMenu =
      HomeMenu({{"Music", [&]() { currentMenu = &musicMenu; }},
                {"YouTube", []() {}},
                {"Spend Recorder", []() {}},
                {"AudioBooks", []() {}},
                {"Media Library", []() {}},
                {"Stopwatch", [&]() { currentMenu = &stopwatchMenu; }},
                {"Alarm", []() {}},
                {"Settings", []() {}}});

  currentMenu = &homeMenu;
  currentMenu->render();
  PiPIO::refreshDisplay();

  while (1) {
    if (getCurrentMili() % 1000 == 0) {
      currentMenu->onSecondTick();
    }
#ifdef SIM
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYUP) {
        PiPIO::clearDisplay();

        if (event.key.keysym.sym == SDLK_j)
          currentMenu->onInput(1, 0, 0, 0);
        if (event.key.keysym.sym == SDLK_k)
          currentMenu = &homeMenu;
        if (event.key.keysym.sym == SDLK_l)
          currentMenu->onInput(0, 0, 1, 0);
        if (event.key.keysym.sym == SDLK_SEMICOLON)
          currentMenu->onInput(0, 0, 0, 1);

        if (event.key.keysym.sym == SDLK_q) {
          PiPIO::end();
          std::exit(1);
        }

        currentMenu->render();
        PiPIO::refreshDisplay();
      }
    }
#else
    Buttons currentButtons = PiPIO::getButtons();

    bool buttonReleased = false;

    if (prevButtons.a && !currentButtons.a) {
      PiPIO::clearDisplay();
      currentMenu->onInput(1, 0, 0, 0);
      buttonReleased = true;
    }

    if (prevButtons.b && !currentButtons.b) {
      PiPIO::clearDisplay();
      currentMenu = &homeMenu;
      buttonReleased = true;
    }

    if (prevButtons.c && !currentButtons.c) {
      PiPIO::clearDisplay();
      currentMenu->onInput(0, 0, 1, 0);
      buttonReleased = true;
    }

    if (prevButtons.d && !currentButtons.d) {
      PiPIO::clearDisplay();
      currentMenu->onInput(0, 0, 0, 1);
      buttonReleased = true;
    }

    prevButtons = currentButtons;

    if (buttonReleased) {
      currentMenu->render();
      PiPIO::refreshDisplay();
    }

#endif // SIM
    signal(SIGINT, [](int sig) {
      PiPIO::end();
      std::exit(1);
    });
  }
}
