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
  std::vector<YTVideo> videos;

  void createFolders() const;

public:
  YouTubeMenu();

  void render() override;

  void onInput(bool a, bool b, bool c, bool d) override;
};
