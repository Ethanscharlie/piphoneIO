#pragma once

#include <string>
namespace Audio {

void pause();

void play();

void toggle();

void add(const std::string &file);

void next();

void clear();

void clearAndAdd(const std::string &file);

std::string getCurrentFile();

} // namespace Audio
