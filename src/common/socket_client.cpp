


#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "socket_client.h"

using namespace std::chrono; 

namespace common {

int get_cpu_cnt(std::string path) {
  int cpu_count = 0;
  std::ifstream in(path);
  std::string line;
  if (in) {
    while (getline(in, line)) {
      if (boost::algorithm::starts_with(line, "processor")) { cpu_count++;}
    }
  } else {
    spdlog::get("logger")->info("no such file: {}", path);
  }
  return cpu_count;
}

struct cpu_usage {
  int sum() {
    total = user + nice + system + idle + iowait + irq + softirq + stealstolen + guest;
  }
  int user;
  int nice;
  int system;
  int idle;
  int iowait;
  int irq;
  int softirq;
  int stealstolen;
  int guest;
  int total;
};

void cat_cpu(std::string path, cpu_usage& u1){
  std::ifstream in(path);
  std::string line;
  if (!in) {
    spdlog::get("logger")->info("no such file: {}", path);
    return ;
  }
  getline(in, line);
  in.close();
  std::string t;
  std::stringstream ss(line);
  ss >> t >> u1.user >> u1.nice >> u1.system >> u1.idle >> u1.iowait 
      >> u1.irq >> u1.softirq >> u1.stealstolen >> u1.guest;
  u1.sum();
};

float get_cpu_percent(std::string path) {
  // cpu  499460 766 283024 106234186 3054 0 63095 0 0 0
  cpu_usage cpu_usage1, cpu_usage2;

  cat_cpu(path, cpu_usage1);
  std::this_thread::sleep_for(milliseconds(60));
  
  cat_cpu(path, cpu_usage2);
  float per = ((cpu_usage2.total - cpu_usage1.total) - (cpu_usage2.idle - cpu_usage1.idle)) * 100.0 / (cpu_usage2.total - cpu_usage1.total);
  
  return per;
}

struct mem_usage {
  void set(std::vector<int>& a){
    total = a[0] * 1.0 / 1e6;
    free = a[1] * 1.0 / 1e6;
    avail = a[2] * 1.0 / 1e6;
    buffers = a[3] * 1.0 / 1e6;
    cached = a[4] * 1.0 / 1e6;

    used = total - free - cached - buffers;
    if (used < 0) { used = total - free; }

    percent = (total - avail) * 100.0 / total;

    // total = total / 1024 / 1024 / 1024;
    // used = used / 1024 / 1024 / 1024;
    // free = free / 1024 / 1024 / 1024;
  }
  float total;
  float free;
  float avail;
  float buffers;
  float cached;
  float used;
  float percent;
};

void get_mem_percent(std::string path, mem_usage& mu) {
  // MemTotal:        4059272 kB
  // MemFree:         2481940 kB
  // MemAvailable:    3652756 kB
  // Buffers:           53668 kB
  // Cached:           768612 kB

  std::ifstream in(path);
  std::string line, tmp;
  if (!in) {
    spdlog::get("logger")->info("no such file: {}", path);
    return ;
  }
  std::vector<int> data;
  while (data.size() < 5) {
    getline(in, line);
    std::stringstream ss(line);
    ss >> tmp;
    int v;
    ss >> v;
    data.push_back(v);
  }
  in.close();
  mu.set(data);
}

struct disk_usage {
  void set(std::vector<std::string>& a){
    total = std::stoi(a[0].substr(0, a[0].size() -1));
    used = std::stoi(a[1].substr(0, a[1].size() -1));
    free = std::stoi(a[2].substr(0, a[2].size() -1));
    percent = std::stoi(a[3].substr(0, a[3].size() -1));
  }
  int total;
  int used;
  int free;
  float percent;
};

void get_disk_percent(std::string path,std::string point, disk_usage& du) {
  // /dev/mmcblk0p1   59G   28G   29G  49% /

  std::ifstream in(path);
  std::string line, tmp;
  if (!in) {
    spdlog::get("logger")->info("no such file: {}", path);
    return ;
  }
  std::vector<std::string> data;
  while (getline(in, line)) {
    std::stringstream ss(line);
    std::string s1, s2, s3, s4, s5, s6;
    ss >> s1 >> s2 >> s3 >> s4 >> s5 >> s6;
    if (s6 == point) {
      data = {s2, s3, s4, s5};
      break;
    }
  }
  in.close();
  if (data.size() != 4) { return;}
  du.set(data);
}

int get_net_speed() {
  int speed = 0;
  int64 start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  using namespace httplib;
  Client cli("www.baidu.com");
  cli.set_connection_timeout(2);
  auto res = cli.Get("/");
  int64 end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  if (res){
      int length = res->body.length();
      speed = length * 1e3 / (end_time - start_time) / 1000;
  }
  return speed;
}

Json::Value get_device_info() {

  Json::Value device_info;

  system("df -h > disk.txt");
  int cpu_cnt = get_cpu_cnt("/proc/cpuinfo");
  device_info["cpu_count"] = cpu_cnt;
  device_info["cpu_thread_count"] = cpu_cnt;
  device_info["cpu_percent"] = std::to_string(get_cpu_percent("/proc/stat") / cpu_cnt).substr(0,4);
  mem_usage mu;
  get_mem_percent("/proc/meminfo", mu);
  device_info["memory_total"] = std::to_string(mu.total).substr(0, 4);
  device_info["memory_used"] = std::to_string(mu.used).substr(0, 4);
  device_info["memory_free"] = std::to_string(mu.free).substr(0, 4);
  device_info["memory_percent"] = std::to_string(mu.percent).substr(0, 4);
  
  disk_usage du;
  get_disk_percent("disk.txt", "/", du);
  device_info["disk_total"] = du.total;
  device_info["disk_used"] = du.used;
  device_info["disk_free"] = du.free;
  device_info["disk_percent"] = du.percent;
  device_info["net_speed"] = get_net_speed();
  return device_info;
}

std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data) {
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";

    return out;
}

ConnectionMetadata::ConnectionMetadata(ConfigInfo* cfg_ifo, LogInfo* log_ifo, 
  int id, websocketpp::connection_hdl hdl, std::string uri)
  : m_id(id)
  , m_hdl(hdl)
  , m_status("Connecting")
  , m_uri(uri)
  , m_server("N/A")
  , log_ifo(log_ifo)
  , cfg_ifo(cfg_ifo) { 

  listener["EVENT_TARGET_SCAN"] = std::set<std::string>();   
}

void ConnectionMetadata::on_open(client * c, websocketpp::connection_hdl hdl, void* cli) {
  m_status = "Open";
  spdlog::get("logger")->info("[WebSocket][{}][连接成功]", cfg_ifo->nodeId);

  WebsocketEndpoint* cur_cli = (WebsocketEndpoint*)cli;
  cur_cli->bind_usr(m_id);
  // client::connection_ptr con = c->get_con_from_hdl(hdl);
  // m_server = con->get_response_header("Server");
}

void ConnectionMetadata::on_fail(client * c, websocketpp::connection_hdl hdl) {
  m_status = "Failed";

  client::connection_ptr con = c->get_con_from_hdl(hdl);
  // m_server = con->get_response_header("Server");
  m_error_reason = con->get_ec().message();
  if (log_ifo->log_core_img) spdlog::get("logger")->info("on_fail m_error_reason: {}", m_error_reason);
}

void ConnectionMetadata::on_close(client * c, websocketpp::connection_hdl hdl) {
  m_status = "Closed";
  client::connection_ptr con = c->get_con_from_hdl(hdl);
  std::stringstream s;
  s << "close code: " << con->get_remote_close_code() << " (" 
    << websocketpp::close::status::get_string(con->get_remote_close_code()) 
    << "), close reason: " << con->get_remote_close_reason();
  m_error_reason = s.str();
  if (log_ifo->log_core_img) spdlog::get("logger")->info("on_close m_error_reason: {}", m_error_reason);
}

void ConnectionMetadata::on_message(websocketpp::connection_hdl, client::message_ptr msg, void* cli) {
  Json::Value root;
  Json::Reader reader;
  if (!reader.parse(msg->get_payload(), root)) { return; }
  if (log_ifo->log_core_img) spdlog::get("logger")->info("[WebSocket][{}]收到请求: \n{}", cfg_ifo->nodeId, root.toStyledString());

  std::string type = root["type"].asString();
  std::string unique_code = root.isMember("unique") ? root["unique"].asString() : "";
  std::string from_usr = root["from"].asString();

  WebsocketEndpoint* cur_cli = (WebsocketEndpoint*)cli;
  if (type == "GET_DEVICE_STATE") {
    Json::Value device_info = get_device_info();
    cur_cli->send_common(m_id, type, std::to_string(cfg_ifo->nodeId), from_usr, unique_code, 200, device_info);
  } 
  else if (boost::algorithm::starts_with(type, "START_EVENT")) {    // "START_EVENT_TARGET_SCAN"
    listener[type.substr(6)].insert(from_usr);

    cur_cli->send_common(m_id, type, std::to_string(cfg_ifo->nodeId), from_usr, unique_code, 200, Json::Value("开始监听成功"));
  } 
  else if (boost::algorithm::starts_with(type, "STOP_EVENT")) {   // STOP_EVENT_TARGET_SCAN
    if (listener[type.substr(5)].count(from_usr)) { listener[type.substr(5)].erase(from_usr); }
    cur_cli->send_common(m_id, type, std::to_string(cfg_ifo->nodeId), from_usr, unique_code, 200, Json::Value("停止监听成功"));
  }
  else if (type == "LED_QUERY") {   // LED
    int id = root["message"]["id"].asInt();
    int cs = root["message"]["controlStatus"].asInt();
    std::string ip = "";
    for (auto& x : cfg_ifo->cameraGroups) {
      for (auto& c : x.meeting_cars) { if (id == c.screen_id) { ip = c.screen_ip; } }
    }
    if (log_ifo->log_core_img) spdlog::get("logger")->info("LED_QUERY id: {}, cs: {}, ip: {}", id, cs, ip);
    // 更新led控制状态
    led_status[id] = LEDControlData(id, 0, cs, ip, "");
  }  
  else if (type == "LED_EDIT") {   // LED
    int id = root["message"]["id"].asInt();
    int cs = root["message"]["controlStatus"].asInt();
    int ns = root["message"]["networkStatus"].asInt();
    std::string text = root["message"]["nowShow"].asString();
    std::string ip = "";
    int screen_width = 0, screen_height = 0;
    for (auto& x : cfg_ifo->cameraGroups) { for (auto& c : x.meeting_cars) { 
      if (id == c.screen_id) { 
        ip = c.screen_ip; screen_width = c.screen_width; screen_height = c.screen_height;
        // 及时更新外部控制状态
        c.is_ctl = cs == 0 ? 1 : 0;
      }
    } }
    if (log_ifo->log_core_img) spdlog::get("logger")->info("LED id: {}, cs: {} ns: {}, ip: {}, text: {}", id, cs, ns, ip, text);
    if (ip.empty()) { spdlog::get("logger")->info("ConnectionMetadata::on_message LED: ip is empty"); return;}
    // update
    led_status[id] = LEDControlData(id, ns, cs, ip, "");

    // 发送文本
    httplib::Client cli(ip.c_str());
    cli.set_connection_timeout(4);
    httplib::Headers headers = {
      {"Content-type", "application/json;charset=utf-8" }
    };

    // auto = 1
    if (cs == 1) {
      std::string msg = LEDControl::generate_led_vsn(text, 1, screen_width, screen_height);
      auto res = cli.Post("/api/program/test.vsn", headers, msg, "application/json");
      msg = "LED大屏修改失败";
      if (res && res->status == 200){ msg = "LED大屏修改成功";}
      cur_cli->send_common(m_id, type, std::to_string(cfg_ifo->nodeId), from_usr, unique_code, 200, Json::Value(msg));
      return;
    }

    std::string msg = LEDControl::generate_led_vsn(text, 0, screen_width, screen_height);
    auto res = cli.Post("/api/program/test.vsn", headers, msg, "application/json");
    msg = "LED大屏修改失败";
    if (res && res->status == 200){ msg = "LED大屏修改成功";}
    cur_cli->send_common(m_id, type, std::to_string(cfg_ifo->nodeId), from_usr, unique_code, 200, Json::Value(msg));
  } 
  else {
    spdlog::get("logger")->info("[WebSocket][{}]收到未知请求: \n{}", cfg_ifo->nodeId, root.toStyledString());
  }

}

WebsocketEndpoint::WebsocketEndpoint(ConfigInfo* cfg_ifo, LogInfo* log_ifo) : 
  m_next_id(0), log_ifo(log_ifo), cfg_ifo(cfg_ifo) {
  m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
  m_endpoint.clear_error_channels(websocketpp::log::elevel::all);
  // m_endpoint.set_access_channels(websocketpp::log::alevel::none);
  // m_endpoint.set_error_channels(websocketpp::log::elevel::none);

  m_endpoint.init_asio();
  m_endpoint.start_perpetual();

  m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
}

WebsocketEndpoint::~WebsocketEndpoint() {
  m_endpoint.stop_perpetual();
  
  for (con_list::const_iterator it = connection_list.begin(); it != connection_list.end(); ++it) {
    if (it->second->get_status() != "Open") {
      // Only close open connections
      continue;
    }
    if (log_ifo->log_core_img) spdlog::get("logger")->info("> Closing connection {}", it->second->get_id());
    
    websocketpp::lib::error_code ec;
    m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
    if (ec) {
      spdlog::get("logger")->info("> Error closing connection {}:{}", it->second->get_id(), ec.message());
    }
  }
  
  m_thread->join();
}

int WebsocketEndpoint::connect(std::string const & uri) {
  websocketpp::lib::error_code ec;
  client::connection_ptr con = m_endpoint.get_connection(uri, ec);
  if (ec) {
      spdlog::get("logger")->info("> Connect initialization error: {}", ec.message());
      return -1;
  }
  if (log_ifo->log_core_img) spdlog::get("logger")->info("> m_endpoint.get_connection ok.");

  int new_id = m_next_id++;
  ConnectionMetadata::ptr metadata_ptr = \
    websocketpp::lib::make_shared<ConnectionMetadata>(cfg_ifo, log_ifo, new_id, con->get_handle(), uri);
  connection_list[new_id] = metadata_ptr;

  con->set_open_handler(websocketpp::lib::bind(
    &ConnectionMetadata::on_open,
    metadata_ptr,
    &m_endpoint,
    websocketpp::lib::placeholders::_1,
    (void*)this
  ));
  con->set_fail_handler(websocketpp::lib::bind(
    &ConnectionMetadata::on_fail,
    metadata_ptr,
    &m_endpoint,
    websocketpp::lib::placeholders::_1
  ));
  con->set_close_handler(websocketpp::lib::bind(
    &ConnectionMetadata::on_close,
    metadata_ptr,
    &m_endpoint,
    websocketpp::lib::placeholders::_1
  ));
  con->set_message_handler(websocketpp::lib::bind(
    &ConnectionMetadata::on_message,
    metadata_ptr,
    websocketpp::lib::placeholders::_1,
    websocketpp::lib::placeholders::_2,
    (void*)this
  ));

  m_endpoint.connect(con);

  return new_id;
}

void WebsocketEndpoint::close(int id, websocketpp::close::status::value code, std::string reason) {
  con_list::iterator metadata_it = connection_list.find(id);
  if (metadata_it == connection_list.end()) {
    spdlog::get("logger")->info("> No connection found with id {}", id);
    return;
  }
  
  websocketpp::lib::error_code ec;
  m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
  if (ec) {
    spdlog::get("logger")->info("> Error initiating close: {}", ec.message());
  }
}

void WebsocketEndpoint::send(int id, std::string message) {
  con_list::iterator metadata_it = connection_list.find(id);
  if (metadata_it == connection_list.end()) {
    spdlog::get("logger")->info("> No connection found with id {}", id);
    return;
  }
  
  websocketpp::lib::error_code ec;
  m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
  if (ec) {
    spdlog::get("logger")->info("> Error sending message: {}", ec.message());
    return;
  }
}

ConnectionMetadata::ptr WebsocketEndpoint::get_metadata(int id) const {
  con_list::const_iterator metadata_it = connection_list.find(id);
  if (metadata_it == connection_list.end()) {
    return ConnectionMetadata::ptr();
  } else {
    return metadata_it->second;
  }
}

void WebsocketEndpoint::send_heart(int id) {
  if (connection_list[id]->get_status() != "Open") { return; }

  Json::Value root;
  int64 cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  srand(time(NULL));

  root["type"] = "HEART";
  root["code"] = 200;
  root["message"] = cfg_ifo->nodeId;
  root["unique"] = rand() % 100000;;
  root["time"] = time_int64_to_string(cur_time);
  root["device_info"] = get_device_info();

  std::string total_m = root["device_info"]["memory_total"].asString();
  std::string use_m = root["device_info"]["memory_used"].asString();
  spdlog::get("logger")->info("get_device_info: memory_total: {}  memory_used: {}\n", total_m, use_m);

  std::string msg = root.toStyledString();
  if (log_ifo->log_core_img) spdlog::get("logger")->info("WebsocketEndpoint::send_heart: \n{}", msg);

  send(id, msg);
}

void WebsocketEndpoint::send_common(int id, std::string type, std::string from_user, 
  std::string to_usr, std::string unique_code, int status_code, Json::Value msg) {
  if (connection_list[id]->get_status() != "Open") { return; }

  Json::Value root;
  int64 cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  
  root["type"] = type;
  root["to"] = to_usr;
  root["from"] = from_user;
  root["message"] = msg;
  root["code"] = status_code;
  root["unique"] = unique_code;
  root["time"] = time_int64_to_string(cur_time);

  std::string text = root.toStyledString();
  if (log_ifo->log_core_img) spdlog::get("logger")->info("send_common: \n{}", text);

  send(id, text);
}

void WebsocketEndpoint::bind_usr(int id) {
  srand(time(NULL));
  Json::Value root;
  int64 cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

  root["type"] = "BIND_CHANNEL_NAME";
  root["code"] = 200;
  root["message"] = cfg_ifo->nodeId;
  root["unique"] = rand() % 100000;
  root["time"] = time_int64_to_string(cur_time);
  std::string msg = root.toStyledString();
  if (log_ifo->log_core_img) spdlog::get("logger")->info("bind_usr: {}\n{}", id, msg);

  send(id, msg);
}

void WebsocketEndpoint::send_vihicle_info(int id, std::string type, int64 time, std::vector<BaseInfoPD*>& infos) {
  if (connection_list[id]->get_status() != "Open") { return; }

  Json::Value root;
  int64 cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

  root["to"] = boost::algorithm::join(connection_list[id]->listener[type], ",");
  root["from"] = cfg_ifo->nodeId;
  root["type"] = type;
  root["time"] = time_int64_to_string(cur_time);
  root["smartbox_id"] = cfg_ifo->nodeId;
  root["cur_time"] = time_int64_to_string(time);
  
  Json::Value target_check_result = Json::Value(Json::arrayValue);
  int cnt = 0;
  for (auto bi : infos) { 
    DetInfoPD* di = (DetInfoPD*)bi;
    Json::Value item;
    item["x0"] = di->det_box.tl().x;
    item["y0"] = di->det_box.tl().y;
    item["x1"] = di->det_box.br().x;
    item["y1"] = di->det_box.br().y;
    item["score"] = di->score;
    item["label"] = di->category;
    item["target_id"] = "ID_20210805_9435248" + std::to_string(cnt++);
    target_check_result.append(item);
  }
  root["target_check_result"] = target_check_result;

  std::string msg = root.toStyledString();
  if (log_ifo->log_core_img) spdlog::get("logger")->info("send_vihicle_info: \n{}", msg);
  send(id, msg);
}


}		// namespace smartbox
