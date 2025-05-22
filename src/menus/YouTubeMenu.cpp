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

void YouTubeMenu::render() {
  if (options.empty()) {
    throw std::logic_error("Menu needs at least one option");
  }

  int rangeMin = 0;
  int rangeMax = options.size();

  if ((options.size() + PiPIO::CHAR_PAD) * PiPIO::CHARPX_HEIGHT >
      PiPIO::DISPLAY_HEIGHT - 15) {
    rangeMin = selection;
    rangeMax = 10;
  }
  for (int i = rangeMin; i < rangeMax; i++) {
    std::string text = "";

    if (i == selection) {
      text += ">";
    } else {
      text += " ";
    }

    text += options[i].first;

    int iPos = i - rangeMin;
    int y = iPos * PiPIO::CHARPX_HEIGHT + PiPIO::CHAR_PAD;
    PiPIO::drawText(0, y, text);
  }
}

void YouTubeMenu::onJoystick(int x, int y) {
  if (y == -1) {
    selection++;
    if (selection == videos.size()) {
      selection = 0;
    }
  }

  else if (y == 1) {
    selection--;
    if (selection < 0) {
      selection = videos.size() - 1;
    }
  }
}

void YouTubeMenu::onAButton() { options[selection].second(); }
