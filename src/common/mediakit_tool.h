#pragma once

#include "base_util/utils.h"
#include "common/base_struct.h"

namespace common {
using namespace httplib;


MediaKitInfo parse_mediakit(Json::Value& item);
Json::Value export_mediakit(MediaKitInfo& mkt);

int api_addDeleteStrategy(MediaKitInfo& mk_ifo, LogInfo *log_ifo);

int api_getRtpInfo(MediaKitInfo& mk_ifo, std::string stream_id, bool& exist, LogInfo *log_ifo);

int api_openRtpServer(MediaKitInfo& mk_ifo, std::string mediaKitServer, std::string stream_id, int& port, LogInfo *log_ifo);

int api_closeRtpServer(MediaKitInfo& mk_ifo, std::string mediaKitServer, std::string stream_id, LogInfo *log_ifo);

int api_startSendRtp(MediaKitInfo& mk_ifo, std::string app, std::string stream_id, std::string dst_ip, int dst_port, std::string rtp_protocol, std::string ssrc, LogInfo *log_ifo);

int api_stopSendRtp(MediaKitInfo& mk_ifo, std::string app, std::string stream_id, std::string ssrc, LogInfo *log_ifo);

int api_getMediaList(MediaKitInfo& mk_ifo, int& media_cnt, std::string app, std::string stream_id, LogInfo *log_ifo);

int api_close_stream(common::MediaKitInfo mk_ifo, std::string app, std::string stream, LogInfo *log_ifo);

int api_addStreamProxy(common::MediaKitInfo mk_ifo, std::string& rtsp_url, std::string app, std::string stream, LogInfo *log_ifo);

}   // namespace smartbox
