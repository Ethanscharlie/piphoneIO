#pragma once

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "tinyxml2.h"

#include "Menu.hpp"
#include "curl/curl.h"

class YouTubeMenu : public Menu {
  struct YTVideo {
    std::string name;
    std::string url;
    std::string channel;
  };

  const std::string rssTemplate =
      "https://www.youtube.com/feeds/videos.xml?channel_id=";

  const std::vector<std::string> channelIDs = {"UC8CsGpP6kVNrWeBVmlJ2UyA"};

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
        return ""; // Return an empty string on error
      }
    }
    return readBuffer; // Return the RSS XML as a string
  }

  void getFeed() {
    std::vector<YTVideo> videos;

    for (std::string channelID : channelIDs) {
      std::string rssPage = getRssXml(rssTemplate + channelID);
      std::cout << rssPage << "\n";
    }
  }

public:
  void render() override {}

  void onInput(bool a, bool b, bool c, bool d) override { getFeed(); }
};
