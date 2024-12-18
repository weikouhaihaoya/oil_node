#pragma once
#include <string>
#include <vector>
#include "common/total_struct.h"

namespace func {

struct ConfigInfo{
  ConfigInfo() {
  }
  ConfigInfo(const ConfigInfo& ci) {
    *this = operator=(ci);
  }
  ConfigInfo& operator=(const ConfigInfo& ci) {
    if (this == &ci) { return *this; }
    ConfigInfo::operator=(ci);
    this->categoryId = ci.categoryId;
    this->mqtt_ifo = ci.mqtt_ifo;
    return *this;
  }

  std::string categoryId;
  MqttInfo mqtt_ifo;
};

} //namespace func