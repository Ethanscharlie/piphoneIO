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

YouTubeMenu::YouTubeMenu() {
  createFolders();
  downloadAllRssXML(channelIDs);
  videos = getFeed(channelIDs);
}

void YouTubeMenu::render() {
  int rangeMin = 0;
  int rangeMax = videos.size();

  if ((videos.size() + PiPIO::CHAR_PAD) * PiPIO::CHARPX_HEIGHT >
      PiPIO::DISPLAY_HEIGHT - 15) {
    rangeMin = selection;
  }

  int y = 16;
  for (int i = rangeMin; i < rangeMax; i++) {
    const YTVideo &video = videos[i];

    std::string bullet = " ";
    if (selection == i) {
      bullet = ">";
    }

    y += PiPIO::CHARPX_HEIGHT + PiPIO::CHAR_PAD;
    PiPIO::drawText(0, y, bullet + video.title);
    y += PiPIO::CHARPX_HEIGHT + PiPIO::CHAR_PAD;
    PiPIO::drawText(20, y, "X " + video.channel);
  }
}

void YouTubeMenu::onInput(bool a, bool b, bool c, bool d) {
  if (!a && !b && !c && d) {
    selection++;
    if (selection == videos.size()) {
      selection = 0;
    }
  }

  if (a && !b && !c && !d) {
    selection--;
    if (selection < 0) {
      selection = videos.size() - 1;
    }
  }

  if (!a && !b && c && !d) {
  }
}
