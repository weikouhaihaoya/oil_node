#include "http_tool.h"

namespace common {


int add_stream_pusher(ConfigInfo& cfg_ifo) {
  Client cli("http://127.0.0.1:9080");
  for (auto& cg : cfg_ifo.cameraGroups) {
    if (cg.cam_ifo.rtspAddress.empty()) { continue; }

    std::string uri = "/index/api/addStreamProxy?vhost=__defaultVhost__&app=live&stream="+cg.cam_ifo.cameraId+"&enable_mp4=0&enable_hls=0&url=" + cg.cam_ifo.rtspAddress;
    auto res1 = cli.Get(uri.c_str());
    if (res1 && res1->status == 200) { spdlog::get("logger")->info("add_stream_pusher addStreamProxy: {}", res1->body); }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    // bool ok = false;
    // while(!ok) {
    //   uri = "/index/api/addStreamPusherProxy?vhost=__defaultVhost__&schema=rtsp&app=live&stream="+std::to_string(cg.cameraID)+"&dst_url=rtsp://122.9.132.193:9554/live/" + std::to_string(cg.cameraID);
    //   auto res2 = cli.Get(uri.c_str());
    //   if (res2 && res2->status == 200) { 
    //     spdlog::get("logger")->info("add_stream_pusher addStreamPusherProxy: {}", res2->body); 
    //     Json::Value root;
    //     parse_json(res2->body, root, false);
    //     if (root["code"] == 0) { ok = true; }
    //   }
    // }

  }
  return 0;
}


int api_addCamera(algo::CameraInfo& cam_ifo, MediaKitInfo& mk_ifo, std::string saveType, std::string type, int debug, int replace_local_ip, LogInfo *log_ifo) {
    Json::Value root;
    root["cameraBrand"] = cam_ifo.cameraBrand;
    root["username"] = cam_ifo.user;
    root["password"] = cam_ifo.passwd;
    root["address"] = debug ? cam_ifo.rtspAddress : cam_ifo.cameraIp;
    root["cameraId"] = cam_ifo.cameraId;
    // media_kit 为客户端时，0表示按需拉流 1表示立即拉流
    root["saveType"] = saveType;

    std::string ret_body, msg = root.toStyledString();
    int ret = post_data_base(ret_body, msg, mk_ifo.mediaKitClient, mk_ifo.mediaKitAPI.addCamera, -1, 1, log_ifo);
    if (log_ifo->log_level_2) {spdlog::get("logger")->info("api_addCamera. post_data_base ret_body:{}", ret_body);}
    if (ret != 0) { return ret; }
    Json::Value ret_json;
    ret = parse_json(ret_body, ret_json, false);
    cam_ifo.rtspAddress = ret_json[type].asString();
    if (replace_local_ip) {
      std::string reg_str = "\\d+\\.\\d+\\.\\d+\\.\\d+";
      boost::regex reg(reg_str);
      boost::sregex_iterator it(cam_ifo.rtspAddress.begin(), cam_ifo.rtspAddress.end(), reg);
      boost::sregex_iterator end;
      for (; it != end; ++it) { boost::algorithm::replace_all(cam_ifo.rtspAddress, it->str(), "127.0.0.1"); }
    }
    return ret;
}

int api_get_gb28181_address(algo::CameraInfo& cam_ifo, LogInfo *log_ifo) {
  int ret = 0;
  std::string uri = util::Format(cam_ifo.gb28181.queryDevice, cam_ifo.gb28181.deviceId);
  std::string ret_body, msg;
  httplib::Params params;
  get_data_base(ret_body, params, cam_ifo.gb28181.httpServer, uri, -1, 0, log_ifo);
  Json::Value ret_json;
  ret = parse_json(ret_body, ret_json, false);
  if (ret != 0 || !ret_json.isMember("code") || ret_json["code"].asInt() != 0) { 
    spdlog::get("logger")->info("ERROR. api_get_gb28181_address {} parse_json ret || ret_json[\"code\"]: {}", uri, (ret != 0 ? ret : ret_json["code"].asInt()));
    return ret != 0 ? ret : ret_json["code"].asInt();
  }

  std::string channelId = "";
  if (ret_json["data"]["sessions"].isArray() && ret_json["data"]["sessions"].size() != 0) {
    if (ret_json["data"]["sessions"][0]["devices"].isArray() && ret_json["data"]["sessions"][0]["devices"].size() != 0) {
      channelId = ret_json["data"]["sessions"][0]["devices"][0]["device_id"].asString();
    }
  }
  if (channelId.empty()) { return -1; }

  int64_t invite_time = ret_json["data"]["sessions"][0]["devices"][0]["invite_time"].asInt64();
  if (invite_time == 0) {
    std::string uri2 = util::Format(cam_ifo.gb28181.playStart, cam_ifo.gb28181.deviceId, channelId);
    std::string ret_body2;
    get_data_base(ret_body2, params, cam_ifo.gb28181.httpServer, uri2, -1, 0, log_ifo);
    Json::Value ret_json2;
    ret = parse_json(ret_body2, ret_json2, false);
    if (ret != 0 || !ret_json2.isMember("code") || ret_json2["code"].asInt() != 0) { 
      spdlog::get("logger")->info("ERROR. api_get_gb28181_address {} parse_json ret || ret_json[\"code\"]: {}", uri, (ret != 0 ? ret : ret_json2["code"].asInt()));
      return ret != 0 ? ret : ret_json2["code"].asInt();
    }
  }

  // 格式化流
  cam_ifo.rtspAddress = util::Format(cam_ifo.gb28181.rtspAddress, cam_ifo.gb28181.deviceId, channelId);
  return ret;
}


int api_kick_and_close_stream(MediaKitInfo mediaKitInfo, algo::CameraGroup* cam_grp, LogInfo *log_ifo) {
  Json::Value pdata;
  pdata["secret"] = mediaKitInfo.secret;
  pdata["local_port"] = 9080;
  std::string ret_body, msg = pdata.toStyledString();
  post_data_base(ret_body, msg, mediaKitInfo.mediaKitServer, mediaKitInfo.mediaKitAPI.kickSessions, -1, 1);

  // pdata["app"] = cam_grp->cam_ifo.cameraId;
  pdata["force"] = true;
  msg = pdata.toStyledString();
  post_data_base(ret_body, msg, mediaKitInfo.mediaKitServer, mediaKitInfo.mediaKitAPI.closeStreams, -1, 1, log_ifo);
  return 0;
}

void send_http_heart(LogInfo *log_ifo, ConfigInfo* cfg_ifo) {
  std::string uri = cfg_ifo->httpServerInfo.uploadHttpHeart;
  if (uri.empty()) { return; }

  Json::Value root;
  root["nodeId"] = cfg_ifo->nodeId;
  // root["deviceInfo"] = get_device_info().toStyledString();
  std::string ret_body, msg = root.toStyledString();
  if (log_ifo->log_level_1) spdlog::get("logger")->info("send_http_heart: \n{}", msg);

  post_data_base(ret_body, msg, cfg_ifo->httpServerInfo.httpServer, uri, -1, 1, log_ifo);
  return ;
}

}