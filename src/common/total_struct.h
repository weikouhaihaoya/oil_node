#pragma once
#include <string>
#include <vector>
#include <set>

#include "base_util/log_lite.h"
#include "common/base_struct.h"


namespace common {

struct PTZGeoInfo {        // 球机地理信息
  PTZGeoInfo():lon(0), lan(0), h(0), dis(0) {}
  PTZGeoInfo(int64_t lon, int64_t lan, int64_t h):
              lon(lon), lan(lan), h(h) {}
  int64_t lon;                 
  int64_t lan;
  int64_t h;

  int64_t dis;
};


struct ThermalInfo {
  int rcvPort;                            // 接收端口
  int alarmThermal;                       // 报警温度
};


struct AlarmInfo {
  int64_t     time;               // happenTime 转换而来 秒级时间辍
  int         alarmLevel;
  std::string eventId;
  std::string nodeId;
  std::string deviceId;
  std::string channelId;
  std::string happenTime;
  std::string algoType;
  std::string alarmDescribe;
  std::string imgUrl;
  std::string videoUrl;
  int         misreportState;
  int         isWarning;
  std::string resultDescribe;
};

struct ModelStrInfo {
  ModelStrInfo(): ok(-1)  {}
  ModelStrInfo(std::string id, std::string type, std::string name): ok(0), id(id), type(type), name(name) {
  }
  std::string id;
  std::string type;
  std::string name;
  int ok;
};

struct ConfigInfo : public common::ConfigInfoBase {
  ConfigInfo(): common::ConfigInfoBase() {
  }
  ConfigInfo(const ConfigInfo& ci) {
    *this = operator=(ci);
  }
  ConfigInfo& operator=(const ConfigInfo& ci) {
    if (this == &ci) { return *this; }
    common::ConfigInfoBase::operator=(ci);
    this->appId = ci.appId;
    this->update_flag = 0;
    return *this;
  }

  std::string appId;
  std::atomic_int update_flag;
};

}   // namespace smartbox
