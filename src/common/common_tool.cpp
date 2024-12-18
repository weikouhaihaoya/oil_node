#include "common_tool.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <iconv/iconv.h>
#endif
namespace common {

cv::Rect json_to_rect_imp(Json::Value &data) {
    cv::Rect rect;
    rect.x = data.get("x", Json::Value(Json::intValue)).asInt();
    rect.y = data.get("y", Json::Value(Json::intValue)).asInt();
    rect.width = data.get("width", Json::Value(Json::intValue)).asInt();
    rect.height = data.get("height", Json::Value(Json::intValue)).asInt();
    return rect;
}

Json::Value rect_to_json_imp(cv::Rect &rect) {
    Json::Value root;
    root["x"] = rect.x;
    root["y"] = rect.y;
    root["width"] = rect.width;
    root["height"] = rect.height;
    return root;
}

std::vector<cv::Point> json_to_region(const Json::Value& root) {
  std::vector<cv::Point> region;
  for (auto j : root) {
    region.push_back(cv::Point(j["x"].asInt(), j["y"].asInt()));
  }
  return region;
}

Json::Value region_to_json(std::vector<cv::Point>& points) {
  Json::Value root = Json::arrayValue;
  for (auto& point : points) {
    Json::Value item;
    item["x"] = point.x;
    item["y"] = point.y;
    root.append(item);
  }
  return root;
}

AiNvrInfo parse_aiNvr(Json::Value& item) {
  AiNvrInfo nvr_ifo;
  nvr_ifo.ainvrServer = item.get("ainvrServer", Json::Value(Json::stringValue)).asString();
  nvr_ifo.ainvrAuthApi = item.get("ainvrAuthApi", Json::Value(Json::stringValue)).asString();
  if (!nvr_ifo.ainvrServer.empty()) nvr_ifo.ok = 0;
  return nvr_ifo;
}

Json::Value export_aiNvr(AiNvrInfo& nvr_ifo) {
  Json::Value item;
  item["ainvrServer"] = nvr_ifo.ainvrServer;
  item["ainvrAuthApi"] = nvr_ifo.ainvrAuthApi;
  return item;
}

AppMonitorInfo parse_appMonitor(Json::Value& item) {
  AppMonitorInfo monitor_ifo;
  monitor_ifo.monitorServer = item.get("monitorServer", Json::Value(Json::stringValue)).asString();
  monitor_ifo.peerRestart = item.get("peerRestart", Json::Value(Json::stringValue)).asString();
  if (!monitor_ifo.monitorServer.empty()) monitor_ifo.ok = 0;
  return monitor_ifo;
}

Json::Value export_appMonitor(AppMonitorInfo& monitor_ifo) {
  Json::Value item;
  item["monitorServer"] = monitor_ifo.monitorServer;
  item["peerRestart"] = monitor_ifo.peerRestart;
  return item;
}

HttpServerInfo parse_httpServer(Json::Value& item) {
  HttpServerInfo http_ifo;
  http_ifo.httpServer = item.get("httpServer", Json::Value(Json::stringValue)).asString();
  http_ifo.uploadAlarmFast = item.get("uploadAlarmFast", Json::Value(Json::stringValue)).asString();
  http_ifo.uploadAlarmSlow = item.get("uploadAlarmSlow", Json::Value(Json::stringValue)).asString();
  http_ifo.checkDevice = item.get("checkDevice", Json::Value(Json::stringValue)).asString();
  http_ifo.uploadHttpHeart = item.get("uploadHttpHeart", Json::Value(Json::stringValue)).asString();
  http_ifo.queryDevice = item.get("queryDevice", Json::Value(Json::stringValue)).asString();
  http_ifo.uploadCameraImg = item.get("uploadCameraImg", Json::Value(Json::stringValue)).asString();
  http_ifo.uploadFile = item.get("uploadFile", Json::Value(Json::stringValue)).asString();
  http_ifo.imageDetect = item.get("imageDetect", Json::Value(Json::stringValue)).asString();
  http_ifo.queryMarkerInfo = item.get("queryMarkerInfo", Json::Value(Json::stringValue)).asString();
  if (!http_ifo.httpServer.empty()) http_ifo.ok = 0;
  return http_ifo;
}

Json::Value export_httpServer(HttpServerInfo& http) {
  Json::Value item;
  item["httpServer"] = http.httpServer;
  item["uploadAlarmFast"] = http.uploadAlarmFast;
  item["uploadAlarmSlow"] = http.uploadAlarmSlow;
  item["checkDevice"] = http.checkDevice;
  item["uploadHttpHeart"] = http.uploadHttpHeart;
  item["queryDevice"] = http.queryDevice;
  item["uploadCameraImg"] = http.uploadCameraImg;
  item["uploadFile"] = http.uploadFile;
  item["imageDetect"] = http.imageDetect;
  item["queryMarkerInfo"] = http.queryMarkerInfo;
  return item;
}


common::DeviceInfo parse_devices(Json::Value item, int utf82gbk) {
  DeviceInfo device_ifo;
  device_ifo.deviceId = item.get("deviceId", Json::Value("")).asString();
  device_ifo.name = item.get("name", Json::Value("")).asString();
  if (utf82gbk) {
    char* src_str = const_cast<char *>(device_ifo.name.c_str());
    char dst_gbk[1024] = {0};
    common::Utf8ToGbk(src_str, strlen(src_str), dst_gbk, sizeof(dst_gbk));
    device_ifo.name = std::string(dst_gbk);
  }
  device_ifo.channelStartId = item.get("channelStartId", Json::Value("")).asString();
  device_ifo.channelCount = item.get("channelCount", Json::Value(0)).asInt();
  device_ifo.ip = item.get("ip", Json::Value("")).asString();
  device_ifo.port = item.get("port", Json::Value(0)).asInt();
  device_ifo.manufacturer = item.get("manufacturer", Json::Value("")).asString();
  device_ifo.model = item.get("model", Json::Value("")).asString();
  device_ifo.deviceType = item.get("deviceType", Json::Value("")).asString();
  device_ifo.deviceAccount = item.get("deviceAccount", Json::Value("")).asString();
  device_ifo.password = item.get("password", Json::Value("")).asString();
  device_ifo.transportType = item.get("transportType", Json::Value("")).asString();
  device_ifo.streamMode = item.get("streamMode", Json::Value("")).asString();
  device_ifo.onLine = item.get("onLine", Json::Value(0)).asInt();
  device_ifo.last_heart_time = item.get("last_heart_time", Json::Value(0)).asInt64();
  if (item.isMember("channels")) {
    for (auto chl_js : item["channels"]) {
      common::ChannelInfo chl_ifo = parse_channel(chl_js, utf82gbk);
      device_ifo.channels.push_back(chl_ifo);
    }
  }
  return device_ifo;
}

Json::Value export_devices(common::DeviceInfo& dev_ifo, int export_chl, int add_device, int gbk2utf8) {
  Json::Value item;
  item["deviceId"] = dev_ifo.deviceId;
  item["name"] = dev_ifo.name;
  if (gbk2utf8) {
    char* src_str = const_cast<char *>(dev_ifo.name.c_str());
    char dst_utf8[1024] = {0};
    common::GbkToUtf8(src_str, strlen(src_str), dst_utf8, sizeof(dst_utf8));
    item["name"] = std::string(dst_utf8);
  }
  item["channelStartId"] = dev_ifo.channelStartId;
  item["channelCount"] = dev_ifo.channels.size();
  item["ip"] = dev_ifo.ip;
  item["port"] = dev_ifo.port;
  item["manufacturer"] = dev_ifo.manufacturer;
  item["model"] = dev_ifo.model;
  item["deviceType"] = dev_ifo.deviceType;
  item["deviceAccount"] = dev_ifo.deviceAccount;
  item["password"] = dev_ifo.password;
  item["transportType"] = dev_ifo.transportType;
  item["streamMode"] = dev_ifo.streamMode;
  item["onLine"] = dev_ifo.onLine;
  item["last_heart_time"] = dev_ifo.last_heart_time;
  // export_chl 只针对 GB28181
  if (dev_ifo.transportType != "GB28181" || export_chl) {
    item["channels"] = Json::arrayValue;
    for (auto chl : dev_ifo.channels) {
      Json::Value chl_js = export_channel(chl, gbk2utf8);
      if (add_device) chl_js["deviceId"] = dev_ifo.deviceId;
      item["channels"].append(chl_js);
    }
  }

  return item;
}


common::ChannelInfo parse_channel(Json::Value item, int utf82gbk) {
  std::string rtspUrl = item.get("rtspUrl", Json::Value("")).asString();
  std::string channelId = item.get("channelId", Json::Value("")).asString();
  int channelIdx = item.get("channelIdx", Json::Value(0)).asInt();
  std::string name = item.get("name", Json::Value("")).asString();
  if (utf82gbk) {
    char* src_str = const_cast<char *>(name.c_str());
    char dst_gbk[1024] = {0};
    common::Utf8ToGbk(src_str, strlen(src_str), dst_gbk, sizeof(dst_gbk));
    name = std::string(dst_gbk);
  }
  int onLine = item.get("onLine", Json::Value(0)).asInt();
  common::ChannelInfo chl_ifo = common::ChannelInfo(channelId, channelIdx, name, rtspUrl, onLine);
  chl_ifo.manufacturer = item.get("manufacturer", Json::Value("")).asString();
  chl_ifo.channelImgTime = item.get("channelImgTime", Json::Value(0)).asInt64();
  chl_ifo.channelImgUrl = item.get("channelImgUrl", Json::Value("")).asString();
  chl_ifo.status = item.get("status", Json::Value("")).asString();
  chl_ifo.last_heart_time = item.get("last_heart_time", Json::Value(0)).asInt64();
  return chl_ifo;
}

Json::Value export_channel(common::ChannelInfo& chl_ifo, int gbk2utf8) {
  Json::Value chl_js;
  chl_js["rtspUrl"] = chl_ifo.rtspUrl;
  chl_js["channelId"] = chl_ifo.channelId;
  chl_js["channelIdx"] = chl_ifo.channelIdx;
  chl_js["name"] = chl_ifo.name;
  if (gbk2utf8) {
    char* src_str = const_cast<char *>(chl_ifo.name.c_str());
    char dst_utf8[1024] = {0};
    common::GbkToUtf8(src_str, strlen(src_str), dst_utf8, sizeof(dst_utf8));
    chl_js["name"] = std::string(dst_utf8);
  }
  chl_js["onLine"] = chl_ifo.onLine;
  chl_js["manufacturer"] = chl_ifo.manufacturer;
  chl_js["channelImgTime"] = chl_ifo.channelImgTime;
  chl_js["channelImgUrl"] = chl_ifo.channelImgUrl;
  chl_js["status"] = chl_ifo.status;
  chl_js["last_heart_time"] = chl_ifo.last_heart_time;
  return chl_js;
}

common::NVRInfo parse_NVRInfo(Json::Value nvr_js) {
  common::NVRInfo nvr_ifo;
  nvr_ifo.NVRIp = nvr_js.get("NVRIp", Json::Value("")).asString();
  nvr_ifo.NVRChannel = nvr_js.get("NVRChannel", Json::Value("")).asString();
  nvr_ifo.NVRBrand = nvr_js.get("NVRBrand", Json::Value("")).asString();
  nvr_ifo.passwd = nvr_js.get("passwd", Json::Value("")).asString();
  nvr_ifo.user = nvr_js.get("user", Json::Value("")).asString();
  if (!nvr_ifo.NVRIp.empty()) {nvr_ifo.ok = 0;}
  return nvr_ifo;
}
Json::Value export_NVRInfo(common::NVRInfo& nvr_ifo) {
  Json::Value item;
  item["NVRIp"] = nvr_ifo.NVRIp;
  item["NVRChannel"] = nvr_ifo.NVRChannel;
  item["NVRBrand"] = nvr_ifo.NVRBrand;
  item["passwd"] = nvr_ifo.passwd;
  item["user"] = nvr_ifo.user;
  return item;
}

#ifdef _WIN32
#else
int GbkToUtf8(char *str_str, size_t src_len, char *dst_str, size_t dst_len)
{
  iconv_t cd;
  char **pin = &str_str;
  char **pout = &dst_str;

  // cd = iconv_open("UCS-2-INTERNAL", "GB18030");
  cd = iconv_open("UTF-8", "GB2312");
  if (cd == nullptr)
    return -2;
  memset(dst_str, 0, dst_len);
  if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
    return -1;
  iconv_close(cd);
  // *pout = (char*)'\0';

  return 0;
}

int Utf8ToGbk(char *src_str, size_t src_len, char *dst_str, size_t dst_len)
{
  iconv_t cd;
  char **pin = &src_str;
  char **pout = &dst_str;

  // cd = iconv_open("GB18030", "UCS-2-INTERNAL"); 
  cd = iconv_open("GB2312", "UTF-8");
  // std::cout << "cd: " << cd << std::endl;
  if (cd == nullptr)
    return -2;
  memset(dst_str, 0, dst_len);
  if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
    return -1;
  iconv_close(cd);
  // *pout = (char*)'\0';

  return 0;
}
#endif

// int tar_file(std::string tar_path, std::vector<bf::path> files) {
//   int ret = -1;
//   TAR* ptar_handle = NULL;

//   // tar
//   ret = tar_open(&ptar_handle, tar_path.c_str(), NULL,  O_WRONLY | O_CREAT,  0644,  TAR_GNU);
//   if (-1 == ret)
//   {
//     spdlog::get("logger")->info("tar_open failed\ntar_path: {}, \nreason: {%s}\n", tar_path, strerror(errno));
//     return ret;
//   }
  
//   for (auto f : files) {
//     tar_append_file(ptar_handle, f.string().c_str(),  f.string().c_str());
//   }
//   tar_append_eof(ptar_handle);
//   tar_close(ptar_handle);

//   // //untar
//   // char* ptar_fname = "tartest.tar";
//   // char* savefdir = "test";
//   // iret = tar_open(&ptar_handle, ptar_fname, NULL,  O_RDONLY,  0644,  TAR_GNU);
//   // if (-1 == iret)
//   // {
//   //     printf("tar_open failed, reason: %s\n", strerror(errno));
//   //     exit(-1);
//   // }
//   // tar_extract_all(ptar_handle, savefdir);
//   // tar_close(ptar_handle);

//   return 0;

// }


}   // namespace smartbox