#pragma once
#include <string>
#include <vector>
#include "common/total_struct.h"

namespace node {

struct NodeConfigInfo : public common::ConfigInfoBase{
  NodeConfigInfo(): common::ConfigInfoBase() {
  }
  NodeConfigInfo(const NodeConfigInfo& ci) {
    *this = operator=(ci);
  }
  NodeConfigInfo& operator=(const NodeConfigInfo& ci) {
    if (this == &ci) { return *this; }
    common::ConfigInfoBase::operator=(ci);
    this->categoryId = ci.categoryId;
    this->mqtt_ifo = ci.mqtt_ifo;
    return *this;
  }

  std::string categoryId;
  common::MqttInfo mqtt_ifo;
};

} //namespace func