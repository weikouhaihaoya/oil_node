#include "func.h"


// 负责快速接收节点发送过来的数据 此处不做任何回应
int AlgoCenter::node_event_mqtt(std::string body, std::string& ret_str) {
  int ret = 0;
  if (log_ifo->log_level_3) spdlog::get("logger")->info("AlgoCenter::node_event_mqtt. body:{}", body);
  
  Json::Value json_data;
  ret = parse_json(body, json_data, false);
  if (ret != 0) { return ret; }
  if (!json_data.isMember("eventType")) { return HTTP_KEY_ERROR;}

  node_mqtt_data_lck.lock();
  node_mqtt_datas.push_back(json_data);
  node_mqtt_data_lck.unlock();

  Json::Value ret_root;
  ret_root["code"] = 200;
  ret_root["msg"] = "ok";
  ret_str = ret_root.toStyledString();
  return 0;
}