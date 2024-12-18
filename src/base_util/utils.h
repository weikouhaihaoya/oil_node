//
// Created by vanlance on 2019-11-04.
//

#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#include <string>
#include <ctime>
#include <iostream>
#include <math.h>
#include <exception>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <time.h>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/random.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "boost/algorithm/hex.hpp"
#include "boost/uuid/detail/md5.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <json/json.h>

#include "base_util/httplib.h"
#include "base_util/error_code.h"
#include "base_util/base64.h"
#include "base_util/log_lite.h"
#include "base_util/format_str.h"
#include "base_util/base_struct.h"

using namespace std::chrono; 
using namespace boost::gregorian;
namespace bf = boost::filesystem;

int return_error(const int code,const std::string &msg, std::string &error_msg);
// matrix implemention

    // in_vector
template <typename T>
int in_vector(T t, const std::vector<T> &vec)
{
  for (size_t i=0; i<vec.size();i++){if (t == vec[i]){return i;}}
  return -1;
}

template <class T>
T find_mode(std::vector<T> &t){
  sort(t.begin(), t.end());
  T maj = t[0];
  int length(0);
  for (size_t i=0;i<t.size();){
      int cur_count = 1;
      T cur_value = t[i];
      for (size_t j=i+1;j<t.size();j++){
          if(cur_value == t[j]){cur_count++;}
          else{break;}
      }
      if(cur_count > length){
          length = cur_count;
          maj = cur_value;
      }
      i += cur_count;
  }
  return maj;
}

// boost filesystem

  // 递归获取文件夹下所有文件夹路径
int get_dir_recurrsive(std::string root, int level, std::vector< std::pair<int, std::string > > &all_files);

  // 递归获取文件夹下所有文件路径
int get_all_files(std::string root, std::vector< std::pair<int, std::string > > all_files);

  // 递归获取文件夹下所有文件路径
int get_files_in_dir_recurrsive(std::string root, int level, std::vector< std::pair<int, std::string > > &all_files);

  // 仅获取当前文件夹下文件路径 不包括当前目录下的文件夹
int get_files_in_dir(std::string dir, std::vector< std::string > &all_files);
int get_files_in_dir(std::string dir,
                                 std::vector< bf::path > &all_files,
                                 std::string exten);
int get_last_dirs(std::string dir, std::vector< bf::path > &all_files, int level=-1);

// 工具

  // 获取 cpu 核心数
int get_CPU_core_num();

std::string get_cpu_adress_x86_64();

  // 判断字符串是否包含指定字符
void split_utf8_string(std::string str, std::vector<std::string>& list);
bool string_contains(std::string &string, std::string c);

  // app过期联网校验
int expiration_date_online(const std::string &end_time);

  // app过期本地校验
int expiration_date_local(const std::string &start_time, const std::string &end_time);

  //生成随机字符串
std::string randstr(int len);

int md5(std::string &str_md5, const char * const buffer, size_t buffer_size, int flag);

int parse_json(const std::string& str, Json::Value& root, bool read_file);

void remove_file(std::string data_dir, time_t keep_seconds, std::string sufix);

void remove_history_img(std::string data_dir, int64_t time_point, int keep_day);

// int tar_file(std::string tar_path, std::vector<bf::path> files);

int get_disk_percent(DiskInfo& du);
std::string get_machine_ip();
float get_board_thermal(CheckDevice device);

// http op
int post_data_base(std::string& ret_body, std::string msg, std::string httpServer, std::string uri, int port=-1, int isPost=1, LogInfo *log_ifo=nullptr, httplib::Headers header=httplib::Headers());
int get_data_base(std::string& ret_body, httplib::Params params, std::string httpServer, std::string uri, int port=-1, int isPost=1, LogInfo *log_ifo=nullptr);
int post_file(std::string& url, std::string httpServer, std::string uri, std::string writer_path, std::string key_name, int read_time=5, int write_time=1, LogInfo *log_ifo=nullptr);
int download_file(std::string updateUrl, std::string httpServer, std::string saveDir,LogInfo *log_ifo=nullptr);

//
void splitUrl(const std::string& url, std::string& host, std::string& path);

// 时间工具
  // 获得服务器日期
int get_server_time(std::string &output_info);

std::string time_int64_to_string(int64_t timepoint, const char * format);

time_t convert_time_str2time_stamp(std::string time_str, const char * format);

#ifdef _WIN32
#else
  // 获取linux下的家目录
std::string get_home_path();
std::string get_linux_ip();
void executeCMD(const char *cmd, std::string& res);
  // 监测进程是否在线
int monitor_process_online(std::string app_name);
  // 监测设备ip是否在线
float monitor_device_online(std::string device_ip);
#endif
  // 监测相机是否可访问专用函数
int check_camera_connectivity(std::string camera_ip);
