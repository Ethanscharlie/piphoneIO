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
  const std::string rssTemplate =
      "https://www.youtube.com/feeds/videos.xml?channel_id=";

  const std::vector<std::string> channelIDs = {"UC8CsGpP6kVNrWeBVmlJ2UyA",
                                               "UCl2mFZoRqjw_ELax4Yisf6w"};

  const std::string rssFolder = "piphone_data/yt/rss/";
  const std::string videoFolder = "piphone_data/yt/videos/";

  int selection = 0;
  std::vector<YTVideo> videos;

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *userp);
  void downloadRssXML(const std::string &url, const std::string &name) const;

  void createFolders() const;

  std::string readFile(const std::string &filepath);

  static std::vector<YTVideo> getChannelFeed(const std::string &rssXML);

  void downloadAllRssXML();

  void getFeed();

public:
  YouTubeMenu();

  void render() override;

  void onInput(bool a, bool b, bool c, bool d) override;
};
