#include "audio.hpp"
#include "io/io.hpp"
#include "menus/HomeMenu.hpp"
#include "menus/ListMenu.hpp"
#include "menus/StopwatchMenu.hpp"
#include "menus/YouTubeMenu.hpp"
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
  // YouTubeMenu youTubeMenu;

  ListMenu musicArtists({});
  std::filesystem::path musicDir = "/home/ethanscharlie/Music/";
  if (std::filesystem::exists(musicDir)) {
    for (const auto &entry : std::filesystem::directory_iterator(musicDir)) {
      if (std::filesystem::is_directory(entry)) {
        std::string name = entry.path().filename().string();
        musicArtists.options.push_back(
            {name, [name, musicDir]() {
               Audio::clear();
               Audio::addList(getAllAudioFiles(musicDir / name));
               Audio::play();
             }});
      }
    }
  } else {
    musicArtists.options.push_back({"Nothing Found.", []() {}});
  }

  ListMenu musicMenu =
      ListMenu({{"Toggle", [&]() { Audio::toggle(); }},
                {"Skip", []() { Audio::next(); }},
                {"Queue All",
                 [musicDir]() {
                   Audio::clear();
                   Audio::addList(getAllAudioFiles(musicDir));
                   Audio::play();
                 }},
                {"Pick Artist", [&]() { currentMenu = &musicArtists; }}});

  HomeMenu homeMenu =
      HomeMenu({{"Music", [&]() { currentMenu = &musicMenu; }},
                // {"YouTube", [&]() { currentMenu = &youTubeMenu; }},
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
      if (event.type == SDL_KEYDOWN) {
        PiPIO::clearDisplay();

        if (event.key.keysym.sym == SDLK_j)
          currentMenu->onAButton();
        if (event.key.keysym.sym == SDLK_k)
          currentMenu->onBButton();
        if (event.key.keysym.sym == SDLK_l)
          currentMenu->onCButton();

        if (event.key.keysym.sym == SDLK_w)
          currentMenu->onJoystick(0, 1);
        if (event.key.keysym.sym == SDLK_s)
          currentMenu->onJoystick(0, -1);
        if (event.key.keysym.sym == SDLK_d)
          currentMenu->onJoystick(1, 0);
        if (event.key.keysym.sym == SDLK_a)
          currentMenu->onJoystick(-1, 0);

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

    signal(SIGINT, [](int sig) {
      PiPIO::end();
      std::exit(1);
    });
#endif // SIM
  }
}
