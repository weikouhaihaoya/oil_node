#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "base_util/utils.h"
#include "base_util/math_compute.h"
#include "camera/hiknet/hikutil.h"
#include "common/common_tool.h"
#include "common/video_tool.h"
#include "common/base_struct.h"
// #include "algo/total_algo.h"

namespace common {

using namespace httplib;

common::GB28181Info parse_gb28181_info(Json::Value gb_js);
Json::Value export_gb28181_info(common::GB28181Info& gb_ifo);
common::WvpInfo parse_wvpInfo(Json::Value& item);
Json::Value export_wvpInfo(common::WvpInfo& wvp_ifo);
common::GBPlatformInfo parse_gbplatform_info(Json::Value gb_js);
Json::Value export_gbplatform_info(common::GBPlatformInfo& gb_ifo);
common::GBServerInfo parse_gbserver_info(Json::Value gb_js);
Json::Value export_gbserver_info(common::GBServerInfo& gb_ifo);
common::GBHttpInfo parse_gbhttp_info(Json::Value& item);
Json::Value export_gbhttp_info(common::GBHttpInfo& http_ifo);

int in_vector_devices(std::string& deviceId, std::vector<common::DeviceInfo>& devices);

int wvp_login(common::WvpInfo& wvp_ifo, LogInfo *log_ifo);

int wvp_play_start(common::WvpInfo& wvp_ifo, std::string deviceId, std::string channelId, std::string& rtsp_url, LogInfo *log_ifo);

// int wvp_platform_query(node::WvpInfo& wvp_ifo, int& cnt, LogInfo *log_ifo);
// int wvp_platform_server_config(node::WvpInfo& wvp_ifo, Json::Value& data, LogInfo *log_ifo);
// int wvp_platform_add(node::WvpInfo& wvp_ifo, LogInfo *log_ifo);
// // 查询国标流通道
// int wvp_platform_channel_list(node::WvpInfo& wvp_ifo, int& chl_cnt, LogInfo *log_ifo);
// // 添加所有国标流通道
// int wvp_platform_update_channel(node::WvpInfo& wvp_ifo, std::string deviceGBId, LogInfo *log_ifo);
// // 查询直播流通道
// int wvp_platform_gbStream_list(node::WvpInfo& wvp_ifo, int& chl_cnt, LogInfo *log_ifo);
// // 添加所有直播流通道
// int wvp_platform_gbStream_add(node::WvpInfo& wvp_ifo, std::string deviceGBId, LogInfo *log_ifo);

// int wvp_proxy_save(node::WvpInfo& wvp_ifo, std::string gb_id, std::string rtsp_url, LogInfo *log_ifo);

int gbapi_query_devices(common::GBHttpInfo& gb_ifo, std::vector<common::DeviceInfo>& devices, LogInfo *log_ifo);

int gbapi_query_channels(common::GBHttpInfo& gb_ifo, std::string deviceId, int utf82gbk, std::vector<common::ChannelInfo>& channels, LogInfo *log_ifo);

int gbapi_play_start(common::GBHttpInfo& gb_ifo, int data_mode, std::string& rtsp_url, std::string deviceId, std::string channelId, LogInfo *log_ifo);

int gbapi_addPlatformQuery(common::GBHttpInfo& gb_ifo, common::GBPlatformInfo& plt_ifo, LogInfo *log_ifo);

// int get_img_by_wvp(node::WvpInfo& wvp_ifo, cv::Mat& img, std::string& rtsp_url, std::string deviceId, std::string channelId, LogInfo *log_ifo);

// int make_snapshot_url(node::WvpInfo& wvp_ifo, common::HttpServerInfo& http_svr_ifo, std::string deviceId, 
//   std::string channelId, std::string& channelImgUrl, LogInfo *log_ifo);
  
}   // namespace cb
