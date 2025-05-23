#include "audio.hpp"
#include "io/io.hpp"
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <queue>
#include <string>
#include <thread>

namespace Audio {

static std::queue<std::string> audioQueue;
static bool playing;
static std::thread *audioThread = nullptr;

//////////////////////////////////////////////////////////////////
// Header functions
//////////////////////////////////////////////////////////////////

void init() {
  SDL_Init(SDL_INIT_AUDIO);

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: "
              << Mix_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }

  audioThread = new std::thread([]() {
    Mix_Chunk *chunk = nullptr;

    while (1) {
      if (Audio::audioQueue.empty())
        continue;

      chunk = Mix_LoadWAV(audioQueue.front().c_str());
      if (chunk == nullptr) {
        std::cout << "Could not load file " << audioQueue.front() << "\n";
        std::cout << Mix_GetError() << "\n";
        audioQueue.pop();
        continue;
      }

      while (!playing) {
        continue;
      }

      Mix_PlayChannel(-1, chunk, 0);
      const std::string currentFile = audioQueue.front();

      while (Mix_Playing(-1)) {
        if (currentFile != audioQueue.front()) { // Skip Button
          break;
        }

        continue;
      }

      Mix_FreeChunk(chunk);
      audioQueue.pop();
    }
  });
}

void pause() {
  playing = false;
  Mix_Pause(-1);
}

void play() {
  if (audioQueue.size() <= 0) {
    std::cerr << "Cannot play, no files in queue\n";
    return;
  }

  playing = true;

  Mix_Resume(-1);
}

void toggle() {
  if (playing) {
    pause();
  } else {
    play();
  }
}

void add(const std::string &file) {
  if (!std::filesystem::exists(file)) {
    std::cerr << "Can't add file, file not found " << file << "\n";
    return;
  }

  audioQueue.push(file);
}

void addList(const std::vector<std::string> &files) {
  for (const std::string &file : files) {
    add(file);
  }
}

void next() {
  if (audioQueue.size() <= 1) {
    std::cerr << "Can't skip, only 1 or less files in queue\n";
    return;
  }

  audioQueue.pop();
}

void clear() {
  playing = false;

  while (!audioQueue.empty()) {
    audioQueue.pop();
  }
}

void clearAndAdd(const std::string &file) {
  clear();
  add(file);
}

std::string getCurrentFile() { return audioQueue.front(); }

} // namespace Audio
