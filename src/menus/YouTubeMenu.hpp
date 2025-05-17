#pragma once

#include <format>
#include <gtest/gtest.h>
#include <string>
#include <vector>

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

    std::string getLine() const {
      return std::format("{} : {}", title, channel);
    }
  };

  const std::string rssTemplate =
      "https://www.youtube.com/feeds/videos.xml?channel_id=";

  const std::vector<std::string> channelIDs = {"UC8CsGpP6kVNrWeBVmlJ2UyA",
                                               "UCl2mFZoRqjw_ELax4Yisf6w"};

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
    std::vector<YTVideo> videos;

    for (const std::string &channelID : channelIDs) {
      const std::string channelURL = rssTemplate + channelID;
      const std::string rssXML = getRssXml(channelURL);
      std::vector<YTVideo> channelVideos = getChannelFeed(rssXML);

      for (const YTVideo &video : channelVideos) {
        videos.push_back(video);
      }
    }

    for (const YTVideo &video : videos) {
      std::cout << video.getLine() << "\n";
    }
  }

public:
  void render() override {}

  void onInput(bool a, bool b, bool c, bool d) override { getFeed(); }
};
