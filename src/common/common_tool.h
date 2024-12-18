#pragma once
#include <opencv2/opencv.hpp>
#include "base_util/utils.h"
#include "common/total_struct.h"
// #include "common/mediakit_tool.h"
// #include "common/gb28181_tool.h"

namespace common {

using namespace httplib;

std::vector<cv::Point> json_to_region(const Json::Value& root);
Json::Value region_to_json(std::vector<cv::Point>& points);

cv::Rect json_to_rect_imp(Json::Value &data);
Json::Value rect_to_json_imp(cv::Rect &rect);

AiNvrInfo parse_aiNvr(Json::Value& item);
Json::Value export_aiNvr(AiNvrInfo& nvr_ifo);

AppMonitorInfo parse_appMonitor(Json::Value& item);
Json::Value export_appMonitor(AppMonitorInfo& monitor_ifo);

HttpServerInfo parse_httpServer(Json::Value& item);
Json::Value export_httpServer(HttpServerInfo& http);


common::NVRInfo parse_NVRInfo(Json::Value nvr_js);
Json::Value export_NVRInfo(common::NVRInfo& nvr_ifo);

common::DeviceInfo parse_devices(Json::Value group_js, int utf82gbk);
// Json::Value export_devices(common::DeviceInfo& dev_ifo, int export_chl=1, int add_device=0, int gbk2utf8=0);
Json::Value export_devices(common::DeviceInfo& dev_ifo, int export_chl, int add_device, int gbk2utf8);
common::ChannelInfo parse_channel(Json::Value item, int utf82gbk);
Json::Value export_channel(common::ChannelInfo& chl_ifo, int gbk2utf8);


#ifdef _WIN32
string GbkToUtf8(const char *src_str);
string Utf8ToGbk(const char *src_str);
#else
int GbkToUtf8(char *str_str, size_t src_len, char *dst_str, size_t dst_len);
int Utf8ToGbk(char *src_str, size_t src_len, char *dst_str, size_t dst_len);
#endif

// int tar_file(std::string tar_path, std::vector<bf::path> files);


}   // namespace smartbox
