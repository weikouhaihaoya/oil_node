#include "gb28181_tool.h"


namespace common {


common::GB28181Info parse_gb28181_info(Json::Value item) {
  common::GB28181Info gb_ifo;
  // GBPlatformInfo
  if (item.isMember("GBPlatformInfo")) {
    gb_ifo.plt_ifo = common::parse_gbplatform_info(item["GBPlatformInfo"]);
  }
  // GBServerInfo
  if (item.isMember("GBServerInfo")) {
    gb_ifo.svr_ifo = common::parse_gbserver_info(item["GBServerInfo"]);
  }
  // GBHttpInfo
  if (item.isMember("GBHttpInfo")) {
    gb_ifo.http_ifo = common::parse_gbhttp_info(item["GBHttpInfo"]);
  }
  // wvpInfo
  if (item.isMember("wvpInfo")) {
    gb_ifo.wvp_ifo = common::parse_wvpInfo(item["wvpInfo"]);
  }
  return gb_ifo;
}

Json::Value export_gb28181_info(common::GB28181Info& gb_ifo) {
  Json::Value item(Json::objectValue);
  // GBPlatformInfo
  if (gb_ifo.plt_ifo.ok == 0) {
    item["GBPlatformInfo"] = common::export_gbplatform_info(gb_ifo.plt_ifo);
  }
  // GBServerInfo
  if (gb_ifo.svr_ifo.ok == 0) {
    item["GBServerInfo"] = common::export_gbserver_info(gb_ifo.svr_ifo);
  }
  // GBHttpInfo
  if (gb_ifo.http_ifo.ok == 0) {
    item["GBHttpInfo"] = common::export_gbhttp_info(gb_ifo.http_ifo);
  }
  // wvpInfo
  if (gb_ifo.wvp_ifo.ok == 0) {
    item["wvpInfo"] = common::export_wvpInfo(gb_ifo.wvp_ifo);
  }
  return item;
}

common::WvpInfo parse_wvpInfo(Json::Value& item) {
  common::WvpInfo wvp_ifo;
  wvp_ifo.serverGBId = item["serverGBId"].asString();
  wvp_ifo.serverGBDomain = item["serverGBDomain"].asString();
  wvp_ifo.serverIP = item["serverIP"].asString();
  wvp_ifo.serverPort = item["serverPort"].asString();
  wvp_ifo.serverpassword = item["serverpassword"].asString();

  wvp_ifo.httpServer = item["httpServer"].asString();
  wvp_ifo.user = item["user"].asString();
  wvp_ifo.password = item["password"].asString();
  wvp_ifo.login = item["login"].asString();
  wvp_ifo.proxySave = item["proxySave"].asString();
  wvp_ifo.queryTransport = item["queryTransport"].asString();
  wvp_ifo.addPlatformAdd = item["addPlatformAdd"].asString();
  wvp_ifo.playStart = item["playStart"].asString();
  wvp_ifo.addPlatformChannelList = item["addPlatformChannelList"].asString();
  wvp_ifo.addPlatformUpdateChannel = item["addPlatformUpdateChannel"].asString();
  wvp_ifo.addPlatformGBStreamList = item["addPlatformGBStreamList"].asString();
  wvp_ifo.addPlatformGBStreamAdd = item["addPlatformGBStreamAdd"].asString();
  wvp_ifo.addPlatformConfig = item["addPlatformConfig"].asString();
  wvp_ifo.addPlatformQuery = item["addPlatformQuery"].asString();
  wvp_ifo.queryDevices = item["queryDevices"].asString();
  wvp_ifo.queryChannels = item["queryChannels"].asString();
  wvp_ifo.ok = 0;
  return wvp_ifo;
}

Json::Value export_wvpInfo(common::WvpInfo& wvp_ifo) {
  Json::Value item;
  item["serverGBId"] = wvp_ifo.serverGBId;
  item["serverGBDomain"] = wvp_ifo.serverGBDomain;
  item["serverIP"] = wvp_ifo.serverIP;
  item["serverPort"] = wvp_ifo.serverPort;
  item["serverpassword"] = wvp_ifo.serverpassword;

  item["httpServer"] = wvp_ifo.httpServer;
  item["user"] = wvp_ifo.user;
  item["password"] = wvp_ifo.password;
  item["login"] = wvp_ifo.login;
  item["proxySave"] = wvp_ifo.proxySave;
  item["queryTransport"] = wvp_ifo.queryTransport;
  item["addPlatformAdd"] = wvp_ifo.addPlatformAdd;
  item["playStart"] = wvp_ifo.playStart;
  item["addPlatformChannelList"] = wvp_ifo.addPlatformChannelList;
  item["addPlatformUpdateChannel"] = wvp_ifo.addPlatformUpdateChannel;
  item["addPlatformGBStreamList"] = wvp_ifo.addPlatformGBStreamList;
  item["addPlatformGBStreamAdd"] = wvp_ifo.addPlatformGBStreamAdd;
  item["addPlatformConfig"] = wvp_ifo.addPlatformConfig;
  item["addPlatformQuery"] = wvp_ifo.addPlatformQuery;
  item["queryDevices"] = wvp_ifo.queryDevices;
  item["queryChannels"] = wvp_ifo.queryChannels;
  return item;
}

common::GBPlatformInfo parse_gbplatform_info(Json::Value item) {
  common::GBPlatformInfo plt_ifo;
  plt_ifo.ok = 0;
  plt_ifo.enable = item["enable"].asString();
  plt_ifo.ProtocolType = item["ProtocolType"].asString();
  plt_ifo.ProtocolVersion = item["ProtocolVersion"].asString();
  plt_ifo.TransportProtocol = item["TransportProtocol"].asString();
  plt_ifo.SIPServerID = item["SIPServerID"].asString();
  plt_ifo.SIPServerIDPrefix = item["SIPServerIDPrefix"].asString();
  plt_ifo.SIPServerIP = item["SIPServerIP"].asString();
  plt_ifo.SIPServerPort = item["SIPServerPort"].asInt();
  plt_ifo.SIPServerPasswd = item["SIPServerPasswd"].asString();
  return plt_ifo;
}
Json::Value export_gbplatform_info(common::GBPlatformInfo& plt_ifo) {
  Json::Value item;
  item["enable"] = plt_ifo.enable;
  item["ProtocolType"] = plt_ifo.ProtocolType;
  item["ProtocolVersion"] = plt_ifo.ProtocolVersion;
  item["TransportProtocol"] = plt_ifo.TransportProtocol;
  item["SIPServerID"] = plt_ifo.SIPServerID;
  item["SIPServerIDPrefix"] = plt_ifo.SIPServerIDPrefix;
  item["SIPServerIP"] = plt_ifo.SIPServerIP;
  item["SIPServerPort"] = plt_ifo.SIPServerPort;
  item["SIPServerPasswd"] = plt_ifo.SIPServerPasswd;
  return item;
}

common::GBServerInfo parse_gbserver_info(Json::Value item) {
  common::GBServerInfo svr_ifo;
  svr_ifo.ok = 0;
  svr_ifo.enable = item["enable"].asString();
  svr_ifo.Manufacture = item["Manufacture"].asString();
  svr_ifo.ProtocolType = item["ProtocolType"].asString();
  svr_ifo.ProtocolVersion = item["ProtocolVersion"].asString();
  svr_ifo.TransportProtocol = item["TransportProtocol"].asString();
  svr_ifo.SIPServerID = item["SIPServerID"].asString();
  svr_ifo.SIPServerIDPrefix = item["SIPServerIDPrefix"].asString();
  svr_ifo.SIPServerIP = item["SIPServerIP"].asString();
  svr_ifo.SIPServerPort = item["SIPServerPort"].asInt();
  svr_ifo.SIPClientIP = item["SIPClientIP"].asString();
  svr_ifo.SIPClientUser = item["SIPClientUser"].asString();
  svr_ifo.SIPClientUserID = item["SIPClientUserID"].asString();
  svr_ifo.SIPClientUserPasswd = item["SIPClientUserPasswd"].asString();
  svr_ifo.SIPClientPort = item["SIPClientPort"].asInt();
  svr_ifo.HeartBeatCycle = item["HeartBeatCycle"].asInt();
  svr_ifo.RegisterValidTime = item["RegisterValidTime"].asInt();
  svr_ifo.RegisterStatus = item["RegisterStatus"].asInt();
  svr_ifo.RegisterInterval = item["RegisterInterval"].asInt();
  svr_ifo.MAXHeartBeatTimeoutTimes = item["MAXHeartBeatTimeoutTimes"].asInt();
  svr_ifo.VideoEncoderID = item["VideoEncoderID"].asString();
  svr_ifo.AudioEncoderID = item["AudioEncoderID"].asString();

  // extra
  svr_ifo.RtpIP = item["RtpIP"].asString();
  svr_ifo.RtpPort = item["RtpPort"].asInt();
  svr_ifo.RtpProtocol = item["RtpProtocol"].asString();
  svr_ifo.ssrc = item["ssrc"].asString();
  return svr_ifo;
}
Json::Value export_gbserver_info(common::GBServerInfo& svr_ifo) {
  Json::Value item;
  item["enable"] = svr_ifo.enable;
  item["Manufacture"] = svr_ifo.Manufacture;
  item["ProtocolType"] = svr_ifo.ProtocolType;
  item["ProtocolVersion"] = svr_ifo.ProtocolVersion;
  item["TransportProtocol"] = svr_ifo.TransportProtocol;
  item["SIPServerID"] = svr_ifo.SIPServerID;
  item["SIPServerIDPrefix"] = svr_ifo.SIPServerIDPrefix;
  item["SIPServerIP"] = svr_ifo.SIPServerIP;
  item["SIPServerPort"] = svr_ifo.SIPServerPort;
  item["SIPClientIP"] = svr_ifo.SIPClientIP;
  item["SIPClientUser"] = svr_ifo.SIPClientUser;
  item["SIPClientUserID"] = svr_ifo.SIPClientUserID;
  item["SIPClientUserPasswd"] = svr_ifo.SIPClientUserPasswd;
  item["SIPClientPort"] = svr_ifo.SIPClientPort;
  item["HeartBeatCycle"] = svr_ifo.HeartBeatCycle;
  item["RegisterValidTime"] = svr_ifo.RegisterValidTime;
  item["RegisterStatus"] = svr_ifo.RegisterStatus;
  item["RegisterInterval"] = svr_ifo.RegisterInterval;
  item["MAXHeartBeatTimeoutTimes"] = svr_ifo.MAXHeartBeatTimeoutTimes;
  item["VideoEncoderID"] = svr_ifo.VideoEncoderID;
  item["AudioEncoderID"] = svr_ifo.AudioEncoderID;

  // extra
  item["RtpIP"] = svr_ifo.RtpIP;
  item["RtpPort"] = svr_ifo.RtpPort;
  item["RtpProtocol"] = svr_ifo.RtpProtocol;
  item["ssrc"] = svr_ifo.ssrc;
  return item;
}

common::GBHttpInfo parse_gbhttp_info(Json::Value& item) {
  common::GBHttpInfo http_ifo;
  // http_ifo.serverGBId = item["serverGBId"].asString();
  // http_ifo.serverGBDomain = item["serverGBDomain"].asString();
  // http_ifo.serverIP = item["serverIP"].asString();
  // http_ifo.serverPort = item["serverPort"].asString();
  // http_ifo.serverpassword = item["serverpassword"].asString();

  http_ifo.ok = 0;
  http_ifo.httpServer = item["httpServer"].asString();
  http_ifo.user = item["user"].asString();
  http_ifo.password = item["password"].asString();
  http_ifo.login = item["login"].asString();
  http_ifo.proxySave = item["proxySave"].asString();
  http_ifo.queryTransport = item["queryTransport"].asString();
  http_ifo.addPlatformAdd = item["addPlatformAdd"].asString();
  http_ifo.playStart = item["playStart"].asString();
  http_ifo.addPlatformChannelList = item["addPlatformChannelList"].asString();
  http_ifo.addPlatformUpdateChannel = item["addPlatformUpdateChannel"].asString();
  http_ifo.addPlatformGBStreamList = item["addPlatformGBStreamList"].asString();
  http_ifo.addPlatformGBStreamAdd = item["addPlatformGBStreamAdd"].asString();
  http_ifo.addPlatformConfig = item["addPlatformConfig"].asString();
  http_ifo.addPlatformQuery = item["addPlatformQuery"].asString();
  http_ifo.addDevices = item["addDevices"].asString();
  http_ifo.delDevices = item["delDevices"].asString();
  http_ifo.queryDevices = item["queryDevices"].asString();
  http_ifo.queryChannels = item["queryChannels"].asString();
  http_ifo.startSendRtp = item["startSendRtp"].asString();
  http_ifo.stopSendRtp = item["stopSendRtp"].asString();
  return http_ifo;
}
Json::Value export_gbhttp_info(common::GBHttpInfo& http_ifo) {
  Json::Value item;
  // item["serverGBId"] = http_ifo.serverGBId;
  // item["serverGBDomain"] = http_ifo.serverGBDomain;
  // item["serverIP"] = http_ifo.serverIP;
  // item["serverPort"] = http_ifo.serverPort;
  // item["serverpassword"] = http_ifo.serverpassword;

  item["httpServer"] = http_ifo.httpServer;
  item["user"] = http_ifo.user;
  item["password"] = http_ifo.password;
  item["login"] = http_ifo.login;
  item["proxySave"] = http_ifo.proxySave;
  item["queryTransport"] = http_ifo.queryTransport;
  item["addPlatformAdd"] = http_ifo.addPlatformAdd;
  item["playStart"] = http_ifo.playStart;
  item["addPlatformChannelList"] = http_ifo.addPlatformChannelList;
  item["addPlatformUpdateChannel"] = http_ifo.addPlatformUpdateChannel;
  item["addPlatformGBStreamList"] = http_ifo.addPlatformGBStreamList;
  item["addPlatformGBStreamAdd"] = http_ifo.addPlatformGBStreamAdd;
  item["addPlatformConfig"] = http_ifo.addPlatformConfig;
  item["addPlatformQuery"] = http_ifo.addPlatformQuery;
  item["addDevices"] = http_ifo.addDevices;
  item["delDevices"] = http_ifo.delDevices;
  item["queryDevices"] = http_ifo.queryDevices;
  item["queryChannels"] = http_ifo.queryChannels;
  item["startSendRtp"] = http_ifo.startSendRtp;
  item["stopSendRtp"] = http_ifo.stopSendRtp;
  return item;
}

int in_vector_devices(std::string& deviceId, std::vector<common::DeviceInfo>& devices) {
  int idx = -1;
  for (int i = 0; i < devices.size(); i++) {
    if (deviceId == devices[i].deviceId) { idx = i; break; }
  }
  return idx;
}

int wvp_login(common::WvpInfo& wvp_ifo, LogInfo *log_ifo) {
  int ret = 0;
  httplib::Client cli(wvp_ifo.httpServer.c_str());
  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(2, 0); 

  httplib::Headers headers = {
    {"Content-type", "application/x-www-form-urlencoded" }
  };

  std::string md5_passwd;
  ret = md5(md5_passwd, wvp_ifo.password.c_str(), wvp_ifo.password.length(), 1);
  boost::algorithm::to_lower(md5_passwd);
  // std::cout << "httpServer:" << wvp_ifo.httpServer << " md5_passwd:" << md5_passwd << std::endl;

  httplib::Params params;
  params.insert(params.end(), std::make_pair("username", wvp_ifo.user));
  params.insert(params.end(), std::make_pair("password", md5_passwd));
  auto res = cli.Get(wvp_ifo.login.c_str(), params, headers);
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("ERROR. wvp_login. Get error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.login, (res ? res->status : int(res.error())));
    return -1;
  }

  Json::Value ret_json;
  ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. wvp_login. \nres->body:{} \nret: {}", res->body, ret);
    return ret;
  }
  if (!ret_json.isMember("data") || !ret_json["data"].isMember("accessToken")){
    spdlog::get("logger")->info("ERROR. wvp_login. key error. \nres->body:{} \nret: {}", res->body, ret);
    return -2;
  }
  wvp_ifo.token = ret_json["data"]["accessToken"].asString();
  return 0;
}

// int wvp_proxy_save(node::WvpInfo& wvp_ifo, std::string gb_id, std::string rtsp_url, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/json" },
//     {"access-token", wvp_ifo.token },
//   };

//   Json::Value root;
//   root["name"] = gb_id;
//   root["type"] = "default";
//   root["app"] = gb_id;
//   root["stream"] = "main";
//   root["url"] = rtsp_url;
//   root["ffmpegCmdKey"] = "ffmpeg.cmd";
//   root["gbId"] = gb_id;
//   root["rtpType"] = "0";
//   root["enable"] = true;
//   root["enableAudio"] = true;
//   root["enableMp4"] = false;
//   root["noneReader"] = "1";
//   root["enableRemoveNoneReader"] = false;
//   root["enableDisableNoneReader"] = true;
//   root["mediaServerId"] = "mediaServer000";
//   std::string msg = root.toStyledString();

//   auto res = cli.Post(wvp_ifo.proxySave.c_str(), headers, msg, "application/json");
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_proxy_save. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.proxySave, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_proxy_save. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }
//   if (!ret_json.isMember("code")){ 
//     spdlog::get("logger")->info("ERROR. wvp_proxy_save. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2; 
//   }
  
//   ret = ret_json["code"].asInt();
//   return ret;
// }

// int wvp_platform_server_config(node::WvpInfo& wvp_ifo, Json::Value& data, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/x-www-form-urlencoded" },
//     {"access-token", wvp_ifo.token },
//   };

//   auto res = cli.Get(wvp_ifo.addPlatformConfig.c_str(), headers);
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_platform_server_config. Get error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.queryDevices, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_platform_server_config. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }

//   if (!ret_json.isMember("data")){
//     spdlog::get("logger")->info("ERROR. wvp_platform_server_config. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2;
//   }
//   data = ret_json["data"];
//   return 0;
// }

// int wvp_platform_query(node::WvpInfo& wvp_ifo, int& cnt, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/x-www-form-urlencoded" },
//     {"access-token", wvp_ifo.token },
//   };

//   auto res = cli.Get(wvp_ifo.addPlatformQuery.c_str(), headers);
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_platform_query. Get error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.addPlatformQuery, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_platform_query. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }

//   if (!ret_json.isMember("data") || !ret_json["data"].isMember("list")){ 
//     spdlog::get("logger")->info("ERROR. wvp_platform_query. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2; 
//   }

//   cnt = ret_json["data"]["list"].size();
//   return 0;
// }


// int wvp_platform_add(node::WvpInfo& wvp_ifo, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/json" },
//     {"access-token", wvp_ifo.token },
//   };

//   Json::Value svr_cfg;
//   if (wvp_platform_server_config(wvp_ifo, svr_cfg, log_ifo) != 0) { return ret;}
//   std::string username = svr_cfg["username"].asString();

//   Json::Value root;
//   root["id"] = Json::nullValue;
//   root["enable"] = true;
//   root["ptz"] = true;
//   root["rtcp"] = false;
//   root["asMessageChannel"] = false;
//   root["autoPushChannel"] = true;
//   root["name"] = username;
//   root["serverGBId"] = wvp_ifo.serverGBId;
//   root["serverGBDomain"] = wvp_ifo.serverGBDomain;
//   root["serverIP"] = wvp_ifo.serverIP;
//   root["serverPort"] = wvp_ifo.serverPort;
//   root["deviceGBId"] = username;
//   root["deviceIp"] = svr_cfg["deviceIp"].asString();
//   root["devicePort"] = svr_cfg["devicePort"].asInt();
//   root["username"] = username;
//   root["password"] = svr_cfg["password"].asString();
//   root["expires"] = 3600;
//   root["keepTimeout"] = 60;
//   root["transport"] = "TCP";
//   root["characterSet"] = "GB2312";
//   root["startOfflinePush"] = false;
//   root["catalogGroup"] = 1;
//   root["administrativeDivision"] = username.substr(0,6);

//   std::string msg = root.toStyledString();
//   if (log_ifo->log_level_3) spdlog::get("logger")->info("wvp_platform_add. msg: \n{}\n", msg);
//   auto res = cli.Post(wvp_ifo.addPlatformAdd.c_str(), headers, msg, "application/json");
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_platform_add. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.addPlatformAdd, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_platform_add. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }
//   if (!ret_json.isMember("code")){ 
//     spdlog::get("logger")->info("ERROR. wvp_platform_add. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2; 
//   }
  
//   ret = ret_json["code"].asInt();
//   return ret;
// }

// int wvp_platform_channel_list(node::WvpInfo& wvp_ifo, int& chl_cnt, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/x-www-form-urlencoded" },
//     {"access-token", wvp_ifo.token },
//   };

//   httplib::Params params;
//   params.insert(params.end(), std::make_pair("page", "1"));
//   params.insert(params.end(), std::make_pair("count", "10"));
//   params.insert(params.end(), std::make_pair("platformId", wvp_ifo.serverGBId));
//   auto res = cli.Get(wvp_ifo.addPlatformChannelList.c_str(), params, headers);
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_platform_channel_list. Get error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.addPlatformChannelList, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_platform_channel_list. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }

//   if (!ret_json.isMember("data") || !ret_json["data"].isMember("list")){ 
//     spdlog::get("logger")->info("ERROR. wvp_platform_channel_list. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2; 
//   }

//   chl_cnt = ret_json["data"]["list"].size();
//   return 0;
// }

// int wvp_platform_update_channel(node::WvpInfo& wvp_ifo, std::string deviceGBId, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/json" },
//     {"access-token", wvp_ifo.token },
//   };

//   Json::Value root;
//   root["platformId"] = wvp_ifo.serverGBId;
//   root["all"] = true;
//   root["channelReduces"] = Json::arrayValue;
//   root["catalogId"] = deviceGBId;
//   std::string msg = root.toStyledString();

//   auto res = cli.Post(wvp_ifo.addPlatformUpdateChannel.c_str(), headers, msg, "application/json");
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_platform_update_channel. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.addPlatformUpdateChannel, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_platform_update_channel. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }
//   if (!ret_json.isMember("code")){ 
//     spdlog::get("logger")->info("ERROR. wvp_platform_update_channel. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2; 
//   }
  
//   ret = ret_json["code"].asInt();
//   return ret;
// }

// int wvp_platform_gbStream_list(node::WvpInfo& wvp_ifo, int& chl_cnt, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/x-www-form-urlencoded" },
//     {"access-token", wvp_ifo.token },
//   };

//   httplib::Params params;
//   params.insert(params.end(), std::make_pair("page", "1"));
//   params.insert(params.end(), std::make_pair("count", "10"));
//   params.insert(params.end(), std::make_pair("platformId", wvp_ifo.serverGBId));
//   auto res = cli.Get(wvp_ifo.addPlatformGBStreamList.c_str(), params, headers);
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_platform_gbStream_list. Get error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.addPlatformGBStreamList, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_platform_gbStream_list. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }

//   if (!ret_json.isMember("data") || !ret_json["data"].isMember("list")){ 
//     spdlog::get("logger")->info("ERROR. wvp_platform_gbStream_list. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2; 
//   }

//   chl_cnt = ret_json["data"]["list"].size();
//   return 0;
// }

// int wvp_platform_gbStream_add(node::WvpInfo& wvp_ifo, std::string deviceGBId, LogInfo *log_ifo) {
//   int ret = 0;
//   httplib::Client cli(wvp_ifo.httpServer.c_str());
//   cli.set_connection_timeout(2, 0);
//   cli.set_read_timeout(5, 0); 
//   cli.set_write_timeout(2, 0); 

//   httplib::Headers headers = {
//     {"Content-type", "application/json" },
//     {"access-token", wvp_ifo.token },
//   };

//   Json::Value root;
//   root["platformId"] = wvp_ifo.serverGBId;
//   root["all"] = true;
//   root["gbStreams"] = Json::arrayValue;
//   root["catalogId"] = deviceGBId;
//   std::string msg = root.toStyledString();

//   auto res = cli.Post(wvp_ifo.addPlatformGBStreamAdd.c_str(), headers, msg, "application/json");
//   if (!res || res->status != 200) {
//     spdlog::get("logger")->info("ERROR. wvp_platform_update_channel. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.addPlatformGBStreamAdd, (res ? res->status : int(res.error())));
//     return -1;
//   }

//   Json::Value ret_json;
//   ret = parse_json(res->body, ret_json, false);
//   if (ret != 0) { 
//     spdlog::get("logger")->info("ERROR. wvp_platform_update_channel. \nres->body:{} \nret: {}", res->body, ret);
//     return ret;
//   }
//   if (!ret_json.isMember("code")){ 
//     spdlog::get("logger")->info("ERROR. wvp_platform_update_channel. key error. \nres->body:{} \nret: {}", res->body, ret);
//     return -2; 
//   }
  
//   ret = ret_json["code"].asInt();
//   return ret;
// }

int gbapi_query_devices(common::GBHttpInfo& gb_ifo, std::vector<common::DeviceInfo>& devices, LogInfo *log_ifo) {
  int ret = 0;
  httplib::Client cli(gb_ifo.httpServer.c_str());
  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(2, 0); 

  httplib::Headers headers = {
    // {"Content-type", "application/x-www-form-urlencoded" },
    {"access-token", gb_ifo.token },
  };

  Json::Value root;
  root["page"] = 1;
  root["count"] = 1000;
  std::string msg = root.toStyledString();
  auto res = cli.Post(gb_ifo.queryDevices.c_str(), headers, msg, "application/json");
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("ERROR. gbapi_query_devices. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", gb_ifo.httpServer, gb_ifo.queryDevices, (res ? res->status : int(res.error())));
    return -1;
  }

  Json::Value ret_json;
  ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. gbapi_query_devices. \nres->body:{} \nret: {}", res->body, ret);
    return ret;
  }

  if (!ret_json.isMember("data") || !ret_json["data"].isMember("list")){
    spdlog::get("logger")->info("ERROR. gbapi_query_devices. key error. \nres->body:{} \nret: {}", res->body, ret);
    return -2;
  }

  int64_t cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  for (auto device : ret_json["data"]["list"]) {
    common::DeviceInfo dev;
    dev.ok = 0;
    dev.deviceId = device["deviceId"].asString();
    dev.name = device["name"].asString();
    dev.channelStartId = device["channelStartId"].asString();
    dev.channelCount = device.get("channelCount", Json::Value(0)).asInt();
    dev.ip = device["ip"].asString();
    dev.port = device.get("port", Json::Value(0)).asInt();
    dev.manufacturer = device["manufacturer"].asString();
    dev.deviceType = device["deviceType"].asString();
    dev.deviceAccount = device["deviceAccount"].asString();
    dev.password = device["password"].asString();
    dev.transportType = device["transportType"].asString();
    dev.streamMode = device["streamMode"].asString();
    dev.onLine = device.get("onLine", Json::Value(0)).asInt();
    dev.last_heart_time = cur_time;
    devices.push_back(dev);
  }
  return 0;
}

int gbapi_query_channels(common::GBHttpInfo& gb_ifo, std::string deviceId, int utf82gbk, std::vector<common::ChannelInfo>& channels, LogInfo *log_ifo) {
  int ret = 0;
  httplib::Client cli(gb_ifo.httpServer.c_str());
  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(2, 0); 

  httplib::Headers headers = {
    {"access-token", gb_ifo.token },
  };

  Json::Value root;
  root["deviceId"] = deviceId;
  root["page"] = 1;
  root["count"] = 1000;
  std::string msg = root.toStyledString();
  auto res = cli.Post(gb_ifo.queryChannels.c_str(), headers, msg, "application/json");
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("ERROR. gbapi_query_channels. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", gb_ifo.httpServer, gb_ifo.queryChannels, (res ? res->status : int(res.error())));
    return -1;
  }

  Json::Value ret_json;
  ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. gbapi_query_channels. \nres->body:{} \nret: {}", res->body, ret);
    return ret;
  }

  if (!ret_json.isMember("data") || !ret_json["data"].isMember("list")){
    spdlog::get("logger")->info("ERROR. gbapi_query_channels. key error. \nres->body:{} \nret: {}", res->body, ret);
    return -2;
  }

  for (auto channel : ret_json["data"]["list"]) {
    common::ChannelInfo chl = common::parse_channel(channel, utf82gbk);
    channels.push_back(chl);
  }
  return 0;
}

int gbapi_play_start(common::GBHttpInfo& gb_ifo, int data_mode, std::string& rtsp_url, 
  std::string deviceId, std::string channelId, LogInfo *log_ifo) {
  int ret = 0;
  httplib::Client cli(gb_ifo.httpServer.c_str());
  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(2, 0); 

  Json::Value root;
  root["deviceId"] = deviceId;
  root["channelId"] = channelId;
  std::string msg = root.toStyledString();

  auto res = cli.Post(gb_ifo.playStart.c_str(), msg, "application/json");
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("ERROR. gbapi_play_start. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", gb_ifo.httpServer, gb_ifo.playStart, (res ? res->status : int(res.error())));
    return -1;
  }
  // 直接返回全部数据
  if (data_mode == 1) {rtsp_url = res->body; return 0;}

  if (log_ifo->log_level_3) {spdlog::get("logger")->info("gbapi_play_start. \nres->body:{} \nret: {}", res->body, ret);}
  Json::Value ret_json;
  ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. gbapi_play_start. \nres->body:{} \nret: {}", res->body, ret);
    return ret;
  }

  int ret_code = ret_json.get("code", Json::Value(int(HTTP_KEY_FAULT_ERROR))).asInt();
  if (ret_code != 0 && ret_code != 200) {
    spdlog::get("logger")->info("ERROR. gbapi_play_start {} parse_json ret || ret_json[\"code\"]: {}", gb_ifo.playStart, ret_code);
    return ret;
  }

  if (!ret_json.isMember("data")){
    spdlog::get("logger")->info("ERROR. gbapi_play_start. key error. \nres->body:{} \nret: {}", res->body, ret);
    return -2;
  }
  if (ret_json["data"].isArray() && ret_json["data"].size() > 0) {
    rtsp_url = ret_json["data"][0].get("rtsp", Json::Value("")).asString();
  }
  return 0;
}


int gbapi_addPlatformQuery(common::GBHttpInfo& gb_ifo, common::GBPlatformInfo& plt_ifo, LogInfo *log_ifo) {
  int ret = 0;
  httplib::Client cli(gb_ifo.httpServer.c_str());
  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(2, 0); 

  Json::Value root;
  root["type"] = "GET";
  std::string msg = root.toStyledString();

  auto res = cli.Post(gb_ifo.addPlatformQuery.c_str(), msg, "application/json");
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("ERROR. gbapi_addPlatformQuery. Post error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", gb_ifo.httpServer, gb_ifo.addPlatformQuery, (res ? res->status : int(res.error())));
    return -1;
  }

  Json::Value ret_json;
  ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. gbapi_addPlatformQuery. \nres->body:{} \nret: {}", res->body, ret);
    return ret;
  }

  if (!ret_json.isMember("data")){ 
    spdlog::get("logger")->info("ERROR. gbapi_addPlatformQuery. key error. \nres->body:{} \nret: {}", res->body, ret);
    return -2; 
  }

  plt_ifo = common::parse_gbplatform_info(ret_json["data"]);
  return 0;
}

// int get_img_by_wvp(node::WvpInfo& wvp_ifo, cv::Mat& img, std::string& rtsp_url, std::string deviceId, std::string channelId, LogInfo *log_ifo) {
//   int ret = 0;
//   int try_cnt = 3;
//   while (try_cnt-- > 0 && rtsp_url.empty()) {
//     ret = wvp_play_start(wvp_ifo, deviceId, channelId, rtsp_url, log_ifo);
//   }
//   if (ret != 0 || rtsp_url.empty()) { return DEVICE_CHANNEL_MAKE_SNAPSHOT_ERROR;}

//   ret = common::get_img_by_rtsp(rtsp_url, img);
//   if (ret != 0) {return ret;}
//   return 0;
// }


// int make_snapshot_url(node::WvpInfo& wvp_ifo, common::HttpServerInfo& http_svr_ifo, 
//   std::string deviceId, std::string channelId, std::string& channelImgUrl, LogInfo *log_ifo) {
//   int ret = 0;
//   std::string rtsp_url = "";
//   cv::Mat img;
//   ret = get_img_by_wvp(wvp_ifo, img, rtsp_url, deviceId, channelId, log_ifo);
//   if (ret != 0) {return ret;}

//   int64_t cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//   std::string img_path = log_ifo->get_log_path()+"/" + std::to_string(cur_time) + "." + deviceId +"." + channelId + ".jpg";
//   cv::imwrite(img_path, img);
//   std::string file_url = "";
//   int try_cnt = 3;
//   while (try_cnt-- > 0 && file_url.empty()) { 
//     ret = post_file(file_url, http_svr_ifo.httpServer, http_svr_ifo.uploadFile, img_path, "url", 3, 3, log_ifo);
//   }
//   channelImgUrl = file_url;
//   return 0;
// }

int wvp_play_start(common::WvpInfo& wvp_ifo, std::string deviceId, std::string channelId, std::string& rtsp_url, LogInfo *log_ifo) {
  int ret = 0;
  httplib::Client cli(wvp_ifo.httpServer.c_str());
  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(2, 0); 

  httplib::Headers headers = {
    {"Content-type", "application/x-www-form-urlencoded" },
    {"access-token", wvp_ifo.token },
  };

  std::string uri = util::Format(wvp_ifo.playStart, deviceId, channelId);
  auto res = cli.Get(uri.c_str(), headers);
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("ERROR. wvp_play_start. Get error. \nhttpServer:{} \nuri:{} \nhttp code: {}\n", wvp_ifo.httpServer, wvp_ifo.queryChannels, (res ? res->status : int(res.error())));
    return -1;
  }

  Json::Value ret_json;
  ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. wvp_play_start. \nres->body:{} \nret: {}", res->body, ret);
    return ret;
  }

  int ret_code = ret_json.get("code", Json::Value(int(HTTP_KEY_FAULT_ERROR))).asInt();
  if (ret_code != 0 && ret_code != 200) {
    spdlog::get("logger")->info("ERROR. post_data_base {} parse_json ret || ret_json[\"code\"]: {}", uri, ret_code);
    return ret;
  }

  if (!ret_json.isMember("data") || !ret_json["data"].isMember("rtsp")){
    spdlog::get("logger")->info("ERROR. wvp_play_start. key error. \nres->body:{} \nret: {}", res->body, ret);
    return -2;
  }
  rtsp_url = ret_json["data"].get("rtsp", Json::Value("")).asString();
  return 0;
}

// int api_addCamera(algo::CameraInfo& cam_ifo, MediaKitInfo& mk_ifo, std::string saveType, std::string type, int debug, int replace_local_ip, LogInfo *log_ifo) {
//     Json::Value root;
//     root["cameraBrand"] = cam_ifo.cameraBrand;
//     root["username"] = cam_ifo.user;
//     root["password"] = cam_ifo.passwd;
//     root["address"] = debug ? cam_ifo.rtspAddress : cam_ifo.cameraIp;
//     root["cameraId"] = cam_ifo.cameraId;
//     // media_kit 为客户端时，0表示按需拉流 1表示立即拉流
//     root["saveType"] = saveType;

//     std::string ret_body, msg = root.toStyledString();
//     int ret = post_data_base(ret_body, msg, mk_ifo.mediaKitServer, mk_ifo.mediaKitAPI.addCamera, -1, 1, log_ifo);
//     if (ret != 0) { return ret; }
//     Json::Value ret_json;
//     ret = parse_json(ret_body, ret_json, false);
//     cam_ifo.rtspAddress = ret_json[type].asString();
//     if (replace_local_ip) {
//       std::string reg_str = "\\d+\\.\\d+\\.\\d+\\.\\d+";
//       boost::regex reg(reg_str);
//       boost::sregex_iterator it(cam_ifo.rtspAddress.begin(), cam_ifo.rtspAddress.end(), reg);
//       boost::sregex_iterator end;
//       for (; it != end; ++it) { boost::algorithm::replace_all(cam_ifo.rtspAddress, it->str(), "127.0.0.1"); }
//     }
//     return ret;
// }


}