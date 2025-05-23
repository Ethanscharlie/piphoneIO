#include "audio.hpp"
#include "io/io.hpp"
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

void init() {}

void pause() { playing = false; }

void play() {
  if (audioQueue.size() <= 0) {
    std::cerr << "Cannot play, no files in queue\n";
    return;
  }

  playing = true;
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
