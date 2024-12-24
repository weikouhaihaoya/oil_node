#include "node.base/local_tool.h"


using namespace std::chrono; 
namespace node {

int update_app(std::string version, std::string updateUrl, MonitorAppTask task, LogInfo *log_ifo) {
  // 下载更新包
  int ret = 0;
  auto cli = httplib::Client(task.uApi.httpServer); 

  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(1, 0);

  std::string body;
  auto res = cli.Get(updateUrl.c_str(),
    [&](const char *data, size_t data_length) {
      body.append(data, data_length);
      return true;
  });

  std::string save_path = bf::path(task.appRoot).append(bf::path(updateUrl).filename().string()).string();
  spdlog::get("logger")->info("save_path: {}", save_path);
  
  std::fstream fout(save_path , std::ios::binary | std::ios::out );
  fout.write(body.c_str(), body.length());
  fout.close();

  // 停止程序
  std::string ret_body, msg;
  ret = post_data_base(ret_body, msg, task.aApi.httpServer, task.aApi.stop, -1, 0, log_ifo);
  std::this_thread::sleep_for(seconds(3));

  // 解压
  std::string save_dir = bf::path(task.appRoot).append(version).string();
  if (bf::exists(save_dir)) { bf::remove_all(save_dir); }
  bf::create_directories(save_dir);
  ret = unzip_file(save_path, save_dir);
  spdlog::get("logger")->info("unzipFile: {}, {} -> {}", ret, save_path, task.appRoot);
  
  // 替换
  std::vector< bf::path > all_files;
  get_files_in_dir(save_dir, all_files, std::string(""));
  spdlog::get("logger")->info("all_files.size(): {}", all_files.size());
  for (auto& p : all_files) {
    std::string dst_dir = boost::algorithm::replace_all_copy(p.parent_path().string(), save_dir, task.appRoot);
    if (!bf::exists(dst_dir)) { bf::create_directories(dst_dir); }
    spdlog::get("logger")->info("dst_dir: {}", dst_dir);
    
    std::string dst_path = bf::path(dst_dir).append(p.filename().string()).string();
    spdlog::get("logger")->info("src_path: {}", p.string());
    spdlog::get("logger")->info("dst_path: {}", dst_path);
    if (bf::exists(dst_path)) {  bf::remove(dst_path);}
    bf::copy_file(p, dst_path);
  }
  // 启动程序
  
  return 0;
}
int string_to_struct(const std::string& str, NodeConfigInfo& cfg_ifo, bool read_file) {
  Json::Reader reader;
  Json::Value root;

  if (read_file) {
    std::ifstream ifs;
    ifs.open(str);
    if (!ifs.is_open()) { return file_not_found_error; }
    if (!reader.parse(ifs, root)) { return json_read_parse_error; }
  } else {
    if (!reader.parse(str, root)) { return json_read_parse_error; }
  }
  cfg_ifo.historyDay=root.get("httpPort", Json::Value(60)).asInt();
  cfg_ifo.historyDir=root.get("historyDir", Json::Value(Json::stringValue)).asString();
  cfg_ifo.serialNumber=root.get("serialNumber", Json::Value(Json::stringValue)).asString();
  cfg_ifo.nodeId=root.get("nodeId", Json::Value(Json::stringValue)).asString();
  cfg_ifo.nodeIp=root.get("nodeIp", Json::Value(Json::stringValue)).asString();
  cfg_ifo.nodeName=root.get("nodeName", Json::Value(Json::stringValue)).asString();
  cfg_ifo.configVersion=root.get("configVersion", Json::Value(Json::stringValue)).asString();
  //cfg_ifo.httpPort=root.get("httpPort", Json::Value(60)).asInt();
  
  //cfg_ifo.categoryId = root.get("categoryId", Json::Value("")).asString();
  cfg_ifo.httpServer=root.get("httpServer", Json::Value("")).asString();
  cfg_ifo.env_heart=root.get("env_heart", Json::Value("")).asString();
  cfg_ifo.cpu_heart=root.get("cpu_heart", Json::Value("")).asString();
  cfg_ifo.cam_heart=root.get("cam_heart", Json::Value("")).asString();
  cfg_ifo.img_heart=root.get("img_heart", Json::Value("")).asString();
  cfg_ifo.net_heart=root.get("net_heart",Json::Value("")).asString();
  cfg_ifo.imgDir=root.get("imgDir",Json::Value("")).asString();


  if(root.isMember("device")){
    for(auto t : root["device"]){
      dev_con dev;
      dev.gpio=t.get("gpio", Json::Value(1)).asInt();
      dev.deviceID=t.get("deviceID", Json::Value(Json::stringValue)).asString();
      dev.restart_time=t.get("restart_time", Json::Value(3)).asInt();
      cfg_ifo.Dev_con.push_back(dev);
    }
  }
  if(root.isMember("modbus_device")){
    for(auto t : root["modbus_device"]){
        modbus_con mod;
        mod.modbusIdx=t.get("modbusIdx", Json::Value(60)).asInt();
        mod.modebusType=t.get("modebusType", Json::Value(Json::stringValue)).asString();
        mod.mod_add=t.get("mod_add", Json::Value(60)).asInt();
        mod.readNumbers=t.get("readNumbers", Json::Value(60)).asInt();

        mod.mod_ID=t.get("mod_ID", Json::Value(Json::stringValue)).asString();
        mod.Interval_time=t.get("Interval_time", Json::Value(60)).asInt();
        mod.Baud_rate=t.get("Baud_rate", Json::Value(60)).asInt();
        mod.Data_bit=t.get("Data_bit", Json::Value(60)).asInt();
        mod.Parity=(t.get("Parity", Json::Value(Json::stringValue)).asString())[0];
        mod.Stop_bit=t.get("Stop_bit", Json::Value(60)).asInt();
        //mod.mod_pos=t.get("mod_pos", Json::Value(60)).asInt();
        //mod.env_time=0;
        cfg_ifo.mdb_con.push_back(mod);
    }
  }
  if(root.isMember("camera")){
    for(auto t : root["camera"]){
      camera came;
      came.IP=t.get("IP", Json::Value(Json::stringValue)).asString();
      came.passwd=t.get("passwd", Json::Value(Json::stringValue)).asString();
      came.user=t.get("user", Json::Value(Json::stringValue)).asString();
      came.brand=t.get("brand", Json::Value(Json::stringValue)).asString();
      came.cameraID=t.get("cameraID", Json::Value(Json::stringValue)).asString();
      cfg_ifo.Camera_con.push_back(came);
    }
  }
  if(root.isMember("Network")){
    for(auto t : root["Network"]){
      Network Net;
      Net.IP=t.get("IP",Json::Value("")).asString();
      Net.Interval_time=t.get("Interval_time",Json::Value(30)).asInt();
      cfg_ifo.net.push_back(Net);
    }
  }


  // ainvrInfo
  // if (root.isMember("ainvrInfo") && root["ainvrInfo"].isMember("ainvrServer")) {
  //   cfg_ifo.ainvrInfo = common::parse_aiNvr(root["ainvrInfo"]);
  // }
  // // appMonitorInfo
  // if (root.isMember("appMonitorInfo") && root["appMonitorInfo"].isMember("monitorServer")) {
  //   cfg_ifo.appMonitorInfo = common::parse_appMonitor(root["appMonitorInfo"]);;
  // }
  // // httpServerInfo
  // if (root.isMember("httpServerInfo") && root["httpServerInfo"].isMember("httpServer")) {
  //   cfg_ifo.httpServerInfo = common::parse_httpServer(root["httpServerInfo"]);
  // }
  // // mediaKitInfo
  // if (root.isMember("mediaKitInfo") && root["mediaKitInfo"].isMember("mediaKitServer")) {
  //   cfg_ifo.mediaKitInfo = common::parse_mediakit(root["mediaKitInfo"]);
  // }

// tasks
  // if (root.isMember("tasks")) {
  //   for (auto t : root["tasks"]) { 
  //     MonitorAppTask task;
  //     task.name = t.get("name", Json::Value(Json::stringValue)).asString();
  //     task.loopTime = t.get("loopTime", Json::Value(60)).asInt();
  //     task.maxRunTime = t.get("maxRunTime", Json::Value(60)).asInt();
  //     task.startHour = t.get("startHour", Json::Value(60)).asInt();
  //     task.endHour = t.get("endHour", Json::Value(60)).asInt();
  //     task.startCommand = t.get("startCommand", Json::Value(Json::stringValue)).asString();
  //     task.appRoot = t.get("appRoot", Json::Value(Json::stringValue)).asString();

  //     if (t.isMember("appAPI") && t["appAPI"].isMember("httpServer")) {
  //       task.aApi.httpServer = t["appAPI"].get("httpServer", Json::Value(Json::stringValue)).asString();
  //       task.aApi.heart = t["appAPI"].get("heart", Json::Value(Json::stringValue)).asString();
  //       task.aApi.stop = t["appAPI"].get("stop", Json::Value(Json::stringValue)).asString();
  //       task.aApi.version = t["appAPI"].get("version", Json::Value(Json::stringValue)).asString();
  //       if (!task.aApi.httpServer.empty()) { task.aApi.ok = true; }
  //     }
      
  //     if (t.isMember("updateAPI") && t["updateAPI"].isMember("httpServer")) {
  //       task.uApi.httpServer = t["updateAPI"].get("httpServer", Json::Value(Json::stringValue)).asString();
  //       task.uApi.update = t["updateAPI"].get("update", Json::Value(Json::stringValue)).asString();
  //       if (!task.uApi.httpServer.empty()) { task.uApi.ok = true; }
  //     }

  //     cfg_ifo.tasks.push_back(task);
  //   }
  // }
  
  // mqttInfo
  //if (root.isMember("mqttInfo")) { cfg_ifo.mqtt_ifo = common::parse_mqttInfo(root["mqttInfo"]); }
  // // httpHelperInfo
  // if (root.isMember("httpHelperInfo")) { cfg_ifo.helper_ifo = node::parse_httpCenterInfo(root["httpHelperInfo"]); }
  // // httpCenterInfo
  // if (root.isMember("httpCenterInfo")) { cfg_ifo.center_ifo = node::parse_httpCenterInfo(root["httpCenterInfo"]); }
  
  // // devices
  // if (!root.isMember("devices")) { return json_key_error;}
  // for (auto group_js : root["devices"]) {
  //   common::DeviceInfo device = common::parse_devices(group_js, 0);
  //   cfg_ifo.devices.push_back(device);
  // }
  
  return 0;
}

//整个配置文件重写
int struct_to_string(NodeConfigInfo& cfg_ifo, std::string& str, bool write_file) {
  Json::Value root;
  // root["runMode"] = cfg_ifo.runMode;
  // root["nodeName"] = cfg_ifo.nodeName;
  // root["serialNumber"] = cfg_ifo.serialNumber;
  // root["nodeId"] = cfg_ifo.nodeId;
  // root["nodeIp"] = cfg_ifo.nodeIp;
  // root["mediaDir"] = cfg_ifo.mediaDir;
  // root["historyDir"] = cfg_ifo.historyDir;
  // root["historyDay"] = cfg_ifo.historyDay;
  //root["categoryId"] = cfg_ifo.categoryId;
  root["historyDay"] = cfg_ifo.historyDay;
  root["historyDir"] = cfg_ifo.historyDir;
  root["serialNumber"] = cfg_ifo.serialNumber;
  root["nodeId"] = cfg_ifo.nodeId;
  root["configVersion"] = cfg_ifo.configVersion;
  root["nodeName"] = cfg_ifo.nodeName;
  root["httpServer"] = cfg_ifo.httpServer;
  root["env_heart"] = cfg_ifo.env_heart;
  root["cpu_heart"] = cfg_ifo.cpu_heart;
  root["cam_heart"] = cfg_ifo.cam_heart;
  root["img_heart"] = cfg_ifo.img_heart;
  root["nodeIp"] = cfg_ifo.nodeIp;
  root["net_heart"]=cfg_ifo.net_heart;
  root["imgDir"]=cfg_ifo.imgDir;



  if(cfg_ifo.mdb_con.size()!=0){
    root["modbus_device"]=Json::arrayValue;
    for(int i=0;i<cfg_ifo.mdb_con.size();i++){
        Json::Value device;
        device["modbusIdx"]=cfg_ifo.mdb_con[i].modbusIdx;
        device["modebusType"]=cfg_ifo.mdb_con[i].modebusType;
        device["mod_add"]=cfg_ifo.mdb_con[i].mod_add;
        device["readNumbers"]=cfg_ifo.mdb_con[i].readNumbers;
        device["mod_ID"]=cfg_ifo.mdb_con[i].mod_ID;
        device["Interval_time"]=cfg_ifo.mdb_con[i].Interval_time;
        device["Baud_rate"]=cfg_ifo.mdb_con[i].Baud_rate;
        device["Data_bit"]=cfg_ifo.mdb_con[i].Data_bit;
        device["Parity"]=std::string(1,cfg_ifo.mdb_con[i].Parity);
        device["Stop_bit"]=cfg_ifo.mdb_con[i].Stop_bit;
        //device["env_time"]=cfg_ifo.mdb_con[i].env_time;
        //device["mod_pos"]=cfg_ifo.mdb_con[i].mod_pos;
        root["modbus_device"].append(device);
    }
  }
  if(cfg_ifo.Dev_con.size()!=0){
    root["device"]=Json::arrayValue;
    for(int i=0;i<cfg_ifo.Dev_con.size();i++){
        Json::Value dev;
        dev["gpio"]=cfg_ifo.Dev_con[i].gpio;
        dev["deviceID"]=cfg_ifo.Dev_con[i].deviceID;
        dev["restart_time"]=cfg_ifo.Dev_con[i].restart_time;
        root["device"].append(dev);
    }
  }
  if(cfg_ifo.Camera_con.size()!=0){
    root["camera"]=Json::arrayValue;
    for(int i=0;i<cfg_ifo.Camera_con.size();i++){
      Json::Value dev;
      dev["IP"]=cfg_ifo.Camera_con[i].IP;
      dev["passwd"]=cfg_ifo.Camera_con[i].passwd;
      dev["user"]=cfg_ifo.Camera_con[i].user;
      dev["brand"]=cfg_ifo.Camera_con[i].brand;
      dev["cameraID"]=cfg_ifo.Camera_con[i].cameraID;
      root["camera"].append(dev);
    }

  }
  if(cfg_ifo.net.size()!=0){
    root["Network"]=Json::arrayValue;
    for(int i=0;i<cfg_ifo.net.size();i++){
      Json::Value nett;
      nett["IP"]=cfg_ifo.net[i].IP;
      nett["Interval_time"]=cfg_ifo.net[i].Interval_time;
      root["Network"].append(nett);
    }
  }

  // ainvrInfo
  // if (cfg_ifo.ainvrInfo.ok==0) root["ainvrInfo"] = common::export_aiNvr(cfg_ifo.ainvrInfo);
  // // appMonitorInfo
  // if (cfg_ifo.appMonitorInfo.ok==0) root["appMonitorInfo"] = common::export_appMonitor(cfg_ifo.appMonitorInfo);
  // // httpServerInfo
  // if (cfg_ifo.httpServerInfo.ok==0) root["httpServerInfo"] = common::export_httpServer(cfg_ifo.httpServerInfo);
  // // mediaKitInfo
  // if (cfg_ifo.mediaKitInfo.ok==0) root["mediaKitInfo"] = common::export_mediakit(cfg_ifo.mediaKitInfo);

  // // devices
  // root["devices"] = Json::Value(Json::arrayValue);
  // for (auto& dg : cfg_ifo.devices) {
  //   root["devices"].append(common::export_devices(dg, 1, 0, 0));
  // }

  if (write_file) {
    std::ofstream ofile;
    ofile.open(str);
    ofile << root.toStyledString();
    ofile << std::endl;
    ofile.close();
  } else {
    str = root.toStyledString();
  }
  return 0;
}

}