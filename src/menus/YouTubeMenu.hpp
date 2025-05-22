#pragma once

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "Menu.hpp"

struct YTVideo {
  std::string title;
  std::string url;
  std::string channel;
  std::string published;
  std::string id;
};

class YouTubeMenu : public Menu {
  const std::vector<std::string> channelIDs = {"UC8CsGpP6kVNrWeBVmlJ2UyA",
                                               "UCl2mFZoRqjw_ELax4Yisf6w"};
  int selection = 0;
  std::vector<std::pair<std::string, std::function<void()>>> options;
  std::vector<YTVideo> videos;

  void createFolders() const;
  void download() const;
  void refresh();
  void assembleMenu();

public:
  YouTubeMenu();

  void render() override;
  void onJoystick(int x, int y) override;
  void onAButton() override;
};
