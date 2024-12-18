#pragma once

#include "base_util/utils.h"
#include "common/total_struct.h"

namespace common {
using namespace httplib;


int add_stream_pusher(ConfigInfo& cfg_ifo);

int api_addCamera(algo::CameraInfo& cameraInfo, MediaKitInfo& mediaKitInfo, std::string saveType, std::string type, int debug=0, int replace_local_ip=0, LogInfo *log_ifo=nullptr);

int api_get_gb28181_address(algo::CameraInfo& cam_ifo, LogInfo *log_ifo);

int api_kick_and_close_stream(MediaKitInfo mediaKitInfo, algo::CameraGroup* cam_grp, LogInfo *log_ifo);

void send_http_heart(LogInfo *log_ifo, ConfigInfo* cfg_ifo);

}   // namespace smartbox
