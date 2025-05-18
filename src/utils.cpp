#include "utils.hpp"
#include "menus/YouTubeMenu.hpp"
#include "tinyxml2.h"
#include <curl/curl.h>
#include <fstream>
#include <stdexcept>

long getCurrentMili() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  long milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return milliseconds;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     std::string *userp) {
  size_t totalSize = size * nmemb;
  userp->append((char *)contents, totalSize);
  return totalSize;
}

void downloadRssXML(const std::string &url, const std::string &path) {
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
      throw std::runtime_error(curl_easy_strerror(res));
    }
  }

  std::ofstream file(path);
  file << readBuffer;
  file.close();
}

std::string readFile(const std::string &filepath) {
  std::ifstream file(filepath);
  std::ostringstream ss;
  ss << file.rdbuf();
  std::string str = ss.str();
  return str;
}

std::vector<YTVideo> getChannelFeed(const std::string &rssXML) {
  std::vector<YTVideo> videos;

  tinyxml2::XMLDocument doc;
  doc.Parse(rssXML.c_str());

  tinyxml2::XMLElement *feed = doc.FirstChildElement("feed");

  const std::string channel = feed->FirstChildElement("title")->GetText();

  for (tinyxml2::XMLElement *entry = feed->FirstChildElement("entry");
       entry != nullptr; entry = entry->NextSiblingElement("entry")) {
    const std::string title = entry->FirstChildElement("title")->GetText();
    const std::string url = entry->FirstChildElement("link")->Attribute("href");
    const std::string published =
        entry->FirstChildElement("published")->GetText();
    const std::string id = entry->FirstChildElement("yt:videoId")->GetText();

    videos.push_back({title, url, channel, published, id});
  }

  return videos;
}

void downloadAllRssXML(const std::vector<std::string> &channelIDs) {
  for (const std::string &channelID : channelIDs) {
    const std::string channelURL = RSS_YT_TEMPLATE + channelID;
    downloadRssXML(channelURL, channelID);
  }
}

std::vector<YTVideo> getFeed(const std::vector<std::string> &channelIDs) {
  std::vector<YTVideo> videos;

  for (const std::string &channelID : channelIDs) {
    const std::string filepath = RSS_FOLDER + channelID;
    const std::string rssXML = readFile(filepath);
    std::vector<YTVideo> channelVideos = getChannelFeed(rssXML);

    for (const YTVideo &video : channelVideos) {
      videos.push_back(video);
    }
  }

  return videos;
}
