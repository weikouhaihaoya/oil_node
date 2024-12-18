#include<iostream>
#include "node.rk3568/oil_node.h"
#include"node.rk3568/error_code.h"
#include <iomanip>  
#include <sstream>  
#include <ctime>  
#include <chrono>  
#include"modbus_api.h"
#include<map>
using namespace httplib;
using namespace rk3568;
using namespace common;

int main(int argc, char** argv) {
  int ret = 0;
  
  //OilNode* node=new OilNode();

  //ModbusAPI* mdb_api = new ModbusAPI();
  //int rett = mdb_api->read_data_rtu();
  std::string cfg_path = CFG_NODE_PATH;
  if (!bf::exists(cfg_path)) {
    std::cout << "'./cfg.node.json' is not exists..." << std::endl; return ret; 
  }
  if (argc == 2 && std::string(argv[1]) == "--version"){
    std::cout << "curent version: "<< CUR_VERSION_RK << std::endl;
    return 0;
  }
  int mode = 1;
  if (argc >= 2) { mode = std::stoi(argv[1]);}

  // 读取本地配置
  node::NodeConfigInfo cfg_ifo;
  if ((ret = node::string_to_struct(cfg_path, cfg_ifo, true)) != 0) { 
    std::cout << "'string_to_struct' error: " <<  ret << std::endl; return ret; 
  }


  // for(int i=0;i<cfg_ifo.Dev_con.size();i++){
  //   if(cfg_ifo.Dev_con[i].dev_name.compare("")==0){
  //     continue;
  //   }
  //   std::cout<<"设备："<<cfg_ifo.Dev_con[i].dev_id<<" "<<cfg_ifo.Dev_con[i].dev_name<<std::endl;
  // }
  LogInfo* log_ifo = new LogInfo(mode, cfg_ifo.historyDir, "log"); 
  // std::this_thread::sleep_for(std::chrono::milliseconds(30*1000));
  rk3568::OilNode* noder = new rk3568::OilNode(&cfg_ifo, log_ifo);
  noder->mdb=new ModbusAPI();
  //noder->cam=new rk3568::OilNode::camera_con();

  if(cfg_ifo.mdb_con.size()!=0&&noder->mdb->mdb_config.size()==0){
      noder->get_config();
  }
  //noder->cfg_ifo=new node::NodeConfigInfo();
  noder->dev_map={
      {1,148},
      {2,151},
      {3,152},
      {4,153},
      {5,95},
      {6,28}
      
  };
  // noder->mdb->mod_map={
  //   {"噪声检测器",0},
  //   {"套压计",1},
  //   {"油压计",2},
  //   {"流量计",3},
  //   {"温度计",4},
  //   {"湿度计",5}




  // };
  // ret = noder->init_param();
  // if (ret != 0) {return ret;}

  // // 启动线程开始分析 // 0 则为本地调试模式
  if (mode != 0) {
  //   // if ((ret = act_code->check_expiration(check_cpu_adress)) != 0) { 
  //   //   spdlog::get("logger")->info("smartbox unknow error: {}", ret); return ret;
  //   //   return ret;
  //   // }
    std::thread t2(&rk3568::OilNode::run, noder);
    t2.detach();
    // std::thread t3(&rk3568::OilNode::Grab_picture,noder);
    // t3.detach();
  }

  // 启动http服务端
  Server svr;
  ret = svr.set_mount_point("/data", "./data");
  svr.Get("/hi", [](const Request& req, Response& res) {
    res.set_content("c++ service for zqdl smartbox start ok!", "text/plain");
  });
  svr.Post("/V1.0/alg/debug", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    // if (0 != (ret = noder->debug_alg(req.body, return_msg))) {
    //   return_error(ret, "error", return_msg);
    // }
    res.set_content(return_msg, "application/json");
  });
  // "modbus_device"
  //       {
  //           "modbusIdx":0,
  //           "modebusType":"RS485",
  //           "mod_add": 769,
  //           "readNumbers":2,

  //           "mod_ID":1,
  //           "Interval_time":60,
  //           "Baud_rate":9600,
  //           "Data_bit":8,
  //           "Parity":"N",
  //           "Stop_bit":1
  //       }
  svr.Post("/api/set_modbus", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->setModbus(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    //写进配置文件
    node::struct_to_string(cfg_ifo,cfg_path);
    res.set_content(return_msg, "application/json");
  });
  // {
  //   "box_reboot"=1;
  // }
  svr.Post("/api/boxReboot", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->box_restart(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    res.set_content(return_msg, "application/json");
  });
  // {
  //   "dev_id":1
  //    "dev_name":"dev"
  // }
  svr.Post("/api/devReboot", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->dev_restart(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    res.set_content(return_msg, "application/json");
  });
  // {
  //   "Alarmid":1,
  //   "status":true,
  //   "contrl_cmd":1
  // }
  svr.Post("/api/contrlAlarmDevice", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->ctlAlarmDevice(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    res.set_content(return_msg, "application/json");
  });
  // {
  //   "boxID":1
  // }

  svr.Post("/api/set_Camera", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->set_Camera(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    node::struct_to_string(cfg_ifo,cfg_path);
    res.set_content(return_msg, "application/json");
  });
// "device":  [
//           {
//             "gpio":1,
//             "deviceID":"22"

//         }
//         ,
    
        // {
        //     "gpio":2,
        //     "deviceID":2
        //}
//   ]
  svr.Post("/api/configDevice", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->set_relay(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    node::struct_to_string(cfg_ifo,cfg_path);

    res.set_content(return_msg, "application/json");
  });
  svr.Post("/api/Grab_picture", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->Grab_picture(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    
    res.set_content(return_msg, "application/json");
  });
  // svr.Post("/api/getCpuInfo", [&](const Request& req, Response& res) {
  //   std::string return_msg;
  //   int ret = 0;
  //   if (0 != (ret = noder->getCpuInfo(req.body, return_msg))) {
  //     return_error(ret, "error", return_msg);
  //   }
    
  //   res.set_content(return_msg, "application/json");
  // });
  svr.Post("/api/delete_device", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->delete_device(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    node::struct_to_string(cfg_ifo,cfg_path);
    res.set_content(return_msg, "application/json");
  });
  svr.Post("/api/Network_configuration", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->Network_configuration(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    node::struct_to_string(cfg_ifo,cfg_path);
    res.set_content(return_msg, "application/json");
  });
  svr.Post("/api/delete_Network", [&](const Request& req, Response& res) {
    std::string return_msg;
    int ret = 0;
    if (0 != (ret = noder->delete_Network(req.body, return_msg))) {
      return_error(ret, MyError::GetErrorString(ret), return_msg);
    }
    node::struct_to_string(cfg_ifo,cfg_path);
    res.set_content(return_msg, "application/json");
  });


  // svr.Post("/api/Camera_Status_query", [&](const Request& req, Response& res) {
  //   std::string return_msg;
  //   int ret = 0;
  //   if (0 != (ret = noder->Camera_Status(req.body, return_msg))) {
  //     return_error(ret, "error", return_msg);
  //   }

  //   res.set_content(return_msg, "application/json");
  // });

  // svr.Get("/api/environmentData", [&](const Request& req, Response& res) {
  //   std::string return_msg;
  //   int ret = 0;
  //   Json::Value root;
  //   root["code"] = 0;
  //   root["msg"] = "success";
  //   root["data"]["boxId"]=302;
  //   root["data"]["checkType"]="environmentData";
  //   root["data"]["happenTime"]=getCurrentTime();
  //   root["data"]["current"]=2.0;
  //   root["data"]["temperature"]=mdb_api->data[0];
  //   root["data"]["humidity"]=mdb_api->data[1];
  //   //root["version"] = CUR_VERSION_RK;
  //   return_msg = root.toStyledString();
  //   res.set_content(return_msg, "application/json");
  // });
  // svr.Get("/api/getCpuInfo", [&](const Request& req, Response& res) {
  //   std::string return_msg;
  //   int ret = 0;
  //   Json::Value root;
  //   root["code"] = 0;
  //   root["msg"] = "success";
  //   root["data"]["categoryId"]="";
  //   root["data"]["nodeName"]=cfg_ifo.nodeName;
  //   root["data"]["nodeId"]=cfg_ifo.serialNumber;
  //   root["data"]["nodeVersion"]=CUR_VERSION_RK;
  //   root["data"]["diskInfo"]= "0/0";
  //   DiskInfo du;
  //   ret = get_disk_percent(du);
  //   if (ret == 0) {root["data"]["diskInfo"]= std::to_string(du.used) +"/"+ std::to_string(du.total);}
  //   CheckDevice device = check_orangepi;
  //   root["data"]["nodeThermal"]=std::to_string(get_board_thermal(device));
  //   root["data"]["nodeStatus"]=0;

  //   //root["version"] = CUR_VERSION_RK;
  //   return_msg = root.toStyledString();
  //   res.set_content(return_msg, "application/json");
  // });
  // svr.Post("/api/contrlAlarmDevice", [&](const Request& req, Response& res) {
  //   std::string return_msg;
  //   int ret = 0;
  //   Json::Value root;
  //   root["code"] = 0;
  //   root["msg"] = "success";
  //   root["data"]["status"]=true;
  //   root["data"]["contrl_cmd"]=true;
  //   //root["version"] = CUR_VERSION_RK;
  //   return_msg = root.toStyledString();
  //   res.set_content(return_msg, "application/json");
  // });

  // svr.Post("/api/noiseData", [&](const Request& req, Response& res) {
  //   std::string return_msg;
  //   int ret = 0;
  //   Json::Value root;
  //   root["code"] = 0;
  //   root["msg"] = "success";
  //   root["data"]["noise"]=;
  //   root["data"]["boxId"]=302;
  //   //root["version"] = CUR_VERSION_RK;
  //   return_msg = root.toStyledString();
  //   res.set_content(return_msg, "application/json");
  // });


  // 监听端口
  // spdlog::get("logger")->info("c++ service for xc smartbox start ok!");
  svr.listen("0.0.0.0", API_node_SERVICE_PORT);
  return 0;
}