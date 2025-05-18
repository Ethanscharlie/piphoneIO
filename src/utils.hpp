#pragma once

#include <string>
#include <vector>

const std::string RSS_YT_TEMPLATE =
    "https://www.youtube.com/feeds/videos.xml?channel_id=";
const std::string RSS_FOLDER = "piphone_data/yt/rss/";
const std::string VIDEO_FOLDER = "piphone_data/yt/videos/";

struct YTVideo;

long getCurrentMili();

size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     std::string *userp);

void downloadRssXML(const std::string &url, const std::string &path);

std::string readFile(const std::string &filepath);

std::vector<YTVideo> getChannelFeed(const std::string &rssXML);

void downloadAllRssXML(const std::vector<std::string> &channelIDs);

std::vector<YTVideo> getFeed(const std::vector<std::string> &channelIDs);
