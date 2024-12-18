#pragma once
#include <stdio.h>
#include <signal.h>
#include <atomic>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#include <queue>
#include<fstream>
#include<map>
#include<string>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <ctime>
#include<time.h>
#include <spdlog/spdlog.h>
#include<utility>
#include <iomanip>
#include <sstream>
#include"node.rk3568/error_code.h"
//#include"common/video_tool.h"
//#include "node.rk3568/oil_node.h"
#include "node.base/local_tool.h"
#include "node.rk3568/total_struct.h"
#include "base_util/utils.h"
#include"node.rk3568/modbus_api.h"
#include"base_util/log_lite.h"
#include"base_util/utils.h"
#include<vector>


#include <stdlib.h>
#include <unistd.h>
#include<cmath>
#include <sstream> 
#define GPIO3_A1 97      




#define GPIO3_C1 113
#define GPIO3_C2 114
#define GPIO3_C3 115
#define GPIO4_C5 148
#define GPIO4_C6 149
#define GPIO0_A0 0
#define GPIO3_B5 109
#define GPIO3_C4 116
#define GPIO3_C5 117

 
using namespace cv;
using namespace common;
namespace rk3568 {
class OilNode
{
public:
  OilNode(node::NodeConfigInfo* cfg_info, LogInfo* log_info);
  ~OilNode();

public:
  int run();
  int init_param();

public: 
    void send_node_heart_data();
    //double get_current();
    int check_gpio_status(int gpio);
    std::string get_Data(std::string deviceID);
    std::string getCpuInfo();

    std::string getCurrentTime();
	  int setModbus(std::string body, std::string& ret_str);
    int box_restart(std::string body, std::string& ret_str);
    int dev_restart(std::string body, std::string& ret_str);
    //int Equipment_query(std::string body, std::string& ret_str);
    int ctlAlarmDevice(std::string body, std::string& ret_str);
    int set_Camera(std::string body, std::string& ret_str);
    int Grab_picture(std::string body, std::string& ret_str);
    int get_config();
    int set_relay(std::string body, std::string& ret_str);
    //int OilNode::delete_modbus(std::string body, std::string& ret_str);
    int delete_device(std::string body, std::string& ret_str);
    std::string Network_detection(std::string ip);
    int Network_configuration(std::string body, std::string& ret_str);
    int delete_Network(std::string body, std::string& ret_str);

    std::string Camera_Status();



    struct dev{
        int dev_gpio;
        std::string dev_id;
        int restart_time;
    };
    std::map<int,int> dev_map;
  struct Set_mod{
      int modbusIdx;
      std::string modebusType;
      int mod_add;
      int readNumbers;
      std::string Data_unit;
      std::string Module_name;
      int Interval_time;
      int Baud_rate;
      int Data_bit;
      char Parity;
      int Stop_bit;
      int mod_ID;
  };
  struct camera_con{
      std::string cameraID;
      std::string ip;
      std::string passwd;
      std::string user;
      std::string brand;
      int Grab_time;
  };

typedef struct PACKED         //定义一个cpu occupy的结构体
{

  char name[20];      //定义一个char类型的数组名name有20个元素

  unsigned int user; //定义一个无符号的int类型的user

  unsigned int nice; //定义一个无符号的int类型的nice

  unsigned int system;//定义一个无符号的int类型的system

  unsigned int idle; //定义一个无符号的int类型的idle

}CPU_OCCUPY;

typedef struct PACKED1         //定义一个mem occupy的结构体
{

  char name[20];               //定义一个char类型的数组名name有20个元素

  unsigned long total;

  char name2[20];

  unsigned long free;

}MEM_OCCUPY;
float get_memoccupy (MEM_OCCUPY *mem);//对无类型get函数含有一个形参结构体类弄的指针O
float cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n);
void   get_cpuoccupy (CPU_OCCUPY *cpust); //对无类型get函数含有一个形参结构体类弄的指针O  



private:
  
public: 

  node::NodeConfigInfo* cfg_ifo;
  LogInfo* log_ifo;

  ModbusAPI* mdb;
  std::vector<camera_con> cam;
  std::string cam_ID;
  std::map<std::string,int> status;
  //camera_con* cam;
  //std::vector<ModbusAPI::modbus_con> mdb;
  int BOX_reboot;
private:


};

}