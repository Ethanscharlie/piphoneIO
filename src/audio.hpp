#pragma once

#include <string>
#include <vector>
namespace Audio {

void pause();

void play();

void toggle();

void add(const std::string &file);

void addList(const std::vector<std::string> &files);

void next();

void clear();

void clearAndAdd(const std::string &file);

std::string getCurrentFile();

} // namespace Audio
