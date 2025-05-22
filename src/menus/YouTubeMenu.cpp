#include "YouTubeMenu.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "io/io.hpp"
#include "utils.hpp"

void YouTubeMenu::createFolders() const {
  std::vector<std::string> folders = {"piphone_data", "piphone_data/yt",
                                      RSS_FOLDER, VIDEO_FOLDER};

  for (const std::string &folder : folders) {
    if (!std::filesystem::is_directory(folder) ||
        !std::filesystem::exists(folder)) {
      std::filesystem::create_directory(folder);
    }
  }
}

void YouTubeMenu::download() const {
  for (const YTVideo &video : videos) {
    if (isVideoDownloaded(video.id)) {
      continue;
    }

    downloadVideoAsAudio(video.url, video.id);
  }
}

void YouTubeMenu::refresh() {
  downloadAllRssXML(channelIDs);
  videos = getFeed(channelIDs);
  assembleMenu();
}

void YouTubeMenu::assembleMenu() {
  options.clear();
  options.push_back({"Refresh", [this]() { refresh(); }});
  options.push_back({"Download", [this]() { download(); }});

  for (YTVideo video : videos) {
    options.push_back({video.title, [this]() {}});
  }
}

YouTubeMenu::YouTubeMenu() {
  createFolders();
  refresh();
}
