#pragma once

#include <format>
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "io/io.hpp"
#include "tinyxml2.h"

#include "Menu.hpp"
#include "curl/curl.h"

class YouTubeMenu : public Menu {
  struct YTVideo {
    std::string title;
    std::string url;
    std::string channel;
    std::string published;
    std::string id;
  };

  const std::string rssTemplate =
      "https://www.youtube.com/feeds/videos.xml?channel_id=";

  const std::vector<std::string> channelIDs = {"UC8CsGpP6kVNrWeBVmlJ2UyA",
                                               "UCl2mFZoRqjw_ELax4Yisf6w"};

  int selection = 0;
  std::vector<YTVideo> videos;

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *userp) {
    size_t totalSize = size * nmemb;
    userp->append((char *)contents, totalSize);
    return totalSize;
  }

  static std::string getRssXml(const std::string &url) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);

      if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                  << std::endl;
        return "";
      }
    }
    return readBuffer;
  }

  static std::vector<YTVideo> getChannelFeed(const std::string &rssXML) {
    std::vector<YTVideo> videos;

    tinyxml2::XMLDocument doc;
    doc.Parse(rssXML.c_str());

    tinyxml2::XMLElement *feed = doc.FirstChildElement("feed");

    const std::string channel = feed->FirstChildElement("title")->GetText();

    for (tinyxml2::XMLElement *entry = feed->FirstChildElement("entry");
         entry != nullptr; entry = entry->NextSiblingElement("entry")) {
      const std::string title = entry->FirstChildElement("title")->GetText();
      const std::string url =
          entry->FirstChildElement("link")->Attribute("href");
      const std::string published =
          entry->FirstChildElement("published")->GetText();
      const std::string id = entry->FirstChildElement("yt:videoId")->GetText();

      videos.push_back({title, url, channel, published, id});
    }

    return videos;
  }

  void getFeed() {
    videos.clear();

    for (const std::string &channelID : channelIDs) {
      const std::string channelURL = rssTemplate + channelID;
      const std::string rssXML = getRssXml(channelURL);
      std::vector<YTVideo> channelVideos = getChannelFeed(rssXML);

      for (const YTVideo &video : channelVideos) {
        videos.push_back(video);
      }
    }
  }

public:
  YouTubeMenu() { getFeed(); }

  void render() override {
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

  void onInput(bool a, bool b, bool c, bool d) override {
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
};
