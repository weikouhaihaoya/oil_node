#include "mediakit_tool.h"

namespace common {



MediaKitInfo parse_mediakit(Json::Value& item) {
  MediaKitInfo mkt_ifo;
  for (auto& svr : item.get("mediaKitServers", Json::arrayValue)) {
    mkt_ifo.mediaKitServers.push_back(svr.asString());
  }
  // 初始化
  mkt_ifo.secret = item.get("secret", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitServer = item.get("mediaKitServer", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitClient = item.get("mediaKitClient", Json::Value(Json::stringValue)).asString();
  auto mediaKitAPI = item.get("mediaKitAPI", Json::Value(Json::objectValue));
  mkt_ifo.mediaKitAPI.getRtpInfo = mediaKitAPI.get("getRtpInfo", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.openRtpServer = mediaKitAPI.get("openRtpServer", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.closeRtpServer = mediaKitAPI.get("closeRtpServer", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.startSendRtp = mediaKitAPI.get("startSendRtp", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.getMediaList = mediaKitAPI.get("getMediaList", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.stopSendRtp = mediaKitAPI.get("stopSendRtp", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.addCamera = mediaKitAPI.get("addCamera", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.addStreamProxy = mediaKitAPI.get("addStreamProxy", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.getVideo = mediaKitAPI.get("getVideo", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.cameraProxy = mediaKitAPI.get("cameraProxy", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.closeStreams = mediaKitAPI.get("closeStreams", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.kickSessions = mediaKitAPI.get("kickSessions", Json::Value(Json::stringValue)).asString();
  mkt_ifo.mediaKitAPI.addDeleteStrategy = mediaKitAPI.get("addDeleteStrategy", Json::Value(Json::stringValue)).asString();
  auto deleteStrategy = item.get("deleteStrategy", Json::Value(Json::objectValue));
  mkt_ifo.deleteStrategy.mainStreamDay = deleteStrategy.get("mainStreamDay", Json::Value(Json::intValue)).asInt();
  mkt_ifo.deleteStrategy.mainStreamHour = deleteStrategy.get("mainStreamHour", Json::Value(Json::intValue)).asInt();
  mkt_ifo.deleteStrategy.subStreamDay = deleteStrategy.get("subStreamDay", Json::Value(Json::intValue)).asInt();
  mkt_ifo.deleteStrategy.subStreamHour = deleteStrategy.get("subStreamHour", Json::Value(Json::intValue)).asInt();
  if (!mkt_ifo.mediaKitServer.empty() && !mkt_ifo.mediaKitClient.empty()) mkt_ifo.ok = 0;
  return mkt_ifo;
}

Json::Value export_mediakit(MediaKitInfo& mkt) {
  Json::Value item;
  item = Json::Value(Json::objectValue);
  item["mediaKitServers"] = Json::arrayValue;
  for (auto& svr : mkt.mediaKitServers) {
    item["mediaKitServers"].append(svr);
  }
  item["secret"] = mkt.secret;
  item["mediaKitServer"] = mkt.mediaKitServer;
  item["mediaKitClient"] = mkt.mediaKitClient;
  auto& mka = mkt.mediaKitAPI;
  auto mediaKitAPI = Json::Value(Json::objectValue);
  mediaKitAPI["getRtpInfo"] = mka.getRtpInfo;
  mediaKitAPI["openRtpServer"] = mka.openRtpServer;
  mediaKitAPI["closeRtpServer"] = mka.closeRtpServer;
  mediaKitAPI["startSendRtp"] = mka.startSendRtp;
  mediaKitAPI["getMediaList"] = mka.getMediaList;
  mediaKitAPI["stopSendRtp"] = mka.stopSendRtp;
  mediaKitAPI["addCamera"] = mka.addCamera;
  mediaKitAPI["addStreamProxy"] = mka.addStreamProxy;
  mediaKitAPI["getVideo"] = mka.getVideo;
  mediaKitAPI["cameraProxy"] = mka.cameraProxy;
  mediaKitAPI["closeStreams"] = mka.closeStreams;
  mediaKitAPI["kickSessions"] = mka.kickSessions;
  mediaKitAPI["addDeleteStrategy"] = mka.addDeleteStrategy;
  item["mediaKitAPI"] = mediaKitAPI;
  auto& ds = mkt.deleteStrategy;
  auto deleteStrategy = Json::Value(Json::objectValue);
  deleteStrategy["mainStreamDay"] = ds.mainStreamDay;
  deleteStrategy["mainStreamHour"] = ds.mainStreamHour;
  deleteStrategy["subStreamDay"] = ds.subStreamDay;
  deleteStrategy["subStreamHour"] = ds.subStreamHour;
  item["deleteStrategy"] = deleteStrategy;
  return item;
}


int api_addDeleteStrategy(MediaKitInfo& mk_ifo, LogInfo *log_ifo) {
  if (mk_ifo.ok!=0) { return 0;}
  
  Json::Value root;
  root["mainStreamDay"] = mk_ifo.deleteStrategy.mainStreamDay;
  root["mainStreamHour"] = mk_ifo.deleteStrategy.mainStreamHour;
  root["subStreamDay"] = mk_ifo.deleteStrategy.subStreamDay;
  root["subStreamHour"] = mk_ifo.deleteStrategy.subStreamHour;

  std::string ret_body, msg = root.toStyledString();

  int ret = post_data_base(ret_body, msg, mk_ifo.mediaKitClient, mk_ifo.mediaKitAPI.addDeleteStrategy, -1, 1, log_ifo);
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("api_addDeleteStrategy: ret:{}, msg:{}, ret_body:{}", ret, msg, ret_body);
  return ret;
}

int api_getRtpInfo(MediaKitInfo& mk_ifo, std::string stream_id, bool& exist, LogInfo *log_ifo) {
  int ret = 0;
  if (mk_ifo.ok!=0) { return 0;}
  
  Json::Value root;
  root["secret"] = mk_ifo.secret;
  root["stream_id"] = stream_id;

  std::string ret_body, msg = root.toStyledString();

  ret = post_data_base(ret_body, msg, mk_ifo.mediaKitServer, mk_ifo.mediaKitAPI.getRtpInfo, -1, 1, log_ifo);
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("api_getRtpInfo: ret:{}, msg:{}, ret_body:{}", ret, msg, ret_body);
  if (ret != 0) {return ret;}

  Json::Value ret_json;
  ret = parse_json(ret_body, ret_json, false);

  exist = ret_json["exist"].asBool();
  return 0;
}


int api_openRtpServer(MediaKitInfo& mk_ifo, std::string mediaKitServer, std::string stream_id, int& port, LogInfo *log_ifo) {
  int ret = 0;
  if (mk_ifo.ok!=0) { return 0;}
  
  Json::Value root;
  root["secret"] = mk_ifo.secret;
  root["stream_id"] = stream_id;
  root["re_use_port"] = "0";
  root["port"] = "0";
  root["only_audio"] = "0";
  root["tcp_mode"] = "1";

  std::string ret_body, msg = root.toStyledString();

  ret = post_data_base(ret_body, msg, mediaKitServer, mk_ifo.mediaKitAPI.openRtpServer, -1, 1, log_ifo);
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("api_openRtpServer: ret:{}, msg:{}, ret_body:{}", ret, msg, ret_body);
  if (ret != 0) {return ret;}

  Json::Value ret_json;
  ret = parse_json(ret_body, ret_json, false);

  port = ret_json["port"].asInt();
  return 0;
}

int api_closeRtpServer(MediaKitInfo& mk_ifo, std::string mediaKitServer, std::string stream_id, LogInfo *log_ifo) {
  int ret = 0;
  if (mk_ifo.ok!=0) { return 0;}
  
  Json::Value root;
  root["secret"] = mk_ifo.secret;
  root["stream_id"] = stream_id;

  std::string ret_body, msg = root.toStyledString();

  ret = post_data_base(ret_body, msg, mediaKitServer, mk_ifo.mediaKitAPI.closeRtpServer, -1, 1, log_ifo);
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("api_closeRtpServer: ret:{}, msg:{}, ret_body:{}", ret, msg, ret_body);
  if (ret != 0) {return ret;}
  return 0;
}

int api_startSendRtp(MediaKitInfo& mk_ifo, std::string app, std::string stream_id, std::string dst_ip, int dst_port, std::string rtp_protocol, std::string ssrc, LogInfo *log_ifo) {
  int ret = 0;
  if (mk_ifo.ok!=0) { return 0;}
  
  Json::Value root;
  root["secret"] = mk_ifo.secret;
  root["vhost"] = "__defaultVhost__";
  root["app"] = app;
  root["use_ps"] = "1";
  root["is_udp"] = rtp_protocol == "TCP/RTP/AVP" ? "0" : "1";
  root["pt"] = "96";
  root["stream"] = stream_id;
  root["ssrc"] = ssrc.empty() ? randstr(10) : ssrc;
  root["dst_port"] = std::to_string(dst_port);
  root["only_audio"] = "0";
  root["dst_url"] = dst_ip;

  std::string ret_body, msg = root.toStyledString();
  ret = post_data_base(ret_body, msg, mk_ifo.mediaKitClient, mk_ifo.mediaKitAPI.startSendRtp, -1, 1, log_ifo);
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("api_startSendRtp: ret:{}, msg:{}, ret_body:{}", ret, msg, ret_body);
  if (ret != 0) {return ret;}
  return 0;
}


int api_stopSendRtp(MediaKitInfo& mk_ifo, std::string app, std::string stream_id, std::string ssrc, LogInfo *log_ifo) {
  int ret = 0;
  if (mk_ifo.ok!=0) { return 0;}
  
  Json::Value root;
  root["secret"] = mk_ifo.secret;
  root["vhost"] = "__defaultVhost__";
  root["app"] = app;
  root["stream"] = stream_id;
  root["ssrc"] = ssrc.empty() ? randstr(10) : ssrc;

  std::string ret_body, msg = root.toStyledString();
  ret = post_data_base(ret_body, msg, mk_ifo.mediaKitClient, mk_ifo.mediaKitAPI.stopSendRtp, -1, 1, log_ifo);
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("api_stopSendRtp: ret:{}, msg:{}, ret_body:{}", ret, msg, ret_body);
  if (ret != 0) {return ret;}
  return 0;
}

int api_getMediaList(MediaKitInfo& mk_ifo, int& media_cnt, std::string app, std::string stream_id, LogInfo *log_ifo) {
  int ret = 0;
  if (mk_ifo.ok!=0) { return 0;}
  
  Json::Value root;
  root["secret"] = mk_ifo.secret;
  root["vhost"] = "__defaultVhost__";
  // 国标推流时，rtsp必须就位
  root["schema"] = "rtsp";
  root["app"] = app;
  root["stream"] = stream_id;

  std::string ret_body, msg = root.toStyledString();
  ret = post_data_base(ret_body, msg, mk_ifo.mediaKitClient, mk_ifo.mediaKitAPI.getMediaList, -1, 1, log_ifo);
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("api_getMediaList: ret:{}, msg:{}, ret_body:{}", ret, msg, ret_body);
  if (ret != 0) {return ret;}

  Json::Value ret_json;
  ret = parse_json(ret_body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. post_data_base {} parse_json ret || ret_json[\"code\"]: {}", mk_ifo.mediaKitAPI.getMediaList, ret);
    return -1;  
  }

  int ret_code = ret_json.get("code", Json::Value(-1)).asInt();
  if (ret_code != 0 && ret_code != 200) {
    spdlog::get("logger")->info("ERROR. post_data_base {} parse_json code: {}", mk_ifo.mediaKitAPI.getMediaList, ret_code);
    return -1;  
  }

  if (ret_json.isMember("data") && ret_json["data"].isArray()) {media_cnt = ret_json["data"].size();}
  return 0;
}

int api_close_stream(common::MediaKitInfo mk_ifo, std::string app, std::string stream, LogInfo *log_ifo) {
  Json::Value pdata;
  // pdata["secret"] = "035c73f7-bb6b-4889-a715-d9eb2d1925cc";
  // pdata["local_port"] = 9080;
  // std::string ret_body, msg = pdata.toStyledString();
  // post_data_base(ret_body, msg, mediaKitInfo.mediaKitServer, mediaKitInfo.mediaKitAPI.kickSessions, -1, 1);

  pdata["app"] = app;
  pdata["stream"] = stream;
  pdata["secret"] = mk_ifo.secret;
  pdata["force"] = true;
  std::string ret_body2, msg2 = pdata.toStyledString();
  std::string uri = mk_ifo.mediaKitAPI.closeStreams;
  post_data_base(ret_body2, msg2, mk_ifo.mediaKitClient, uri, -1, 1, log_ifo);
  return 0;
}

int api_addStreamProxy(common::MediaKitInfo mk_ifo, std::string& rtsp_url, std::string app, std::string stream, LogInfo *log_ifo) {
  Json::Value root;
  root["secret"] = mk_ifo.secret;
  root["vhost"] = "__defaultVhost__";
  root["app"] = app;
  root["stream"] = stream;
  root["url"] = rtsp_url;
  root["rtp_type"] = 0;
  root["enable_mp4"] = 1;
  std::string ret_body, msg = root.toStyledString();
  std::string uri = mk_ifo.mediaKitAPI.addStreamProxy;
  return post_data_base(ret_body, msg, mk_ifo.mediaKitClient, uri, -1, 1, log_ifo);
}

}