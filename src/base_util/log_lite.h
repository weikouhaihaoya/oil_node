#pragma once

#include <string>
#include <mutex>
#include <ctime>
#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>
#include <boost/filesystem.hpp>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

// 获取当前时间字符串
std::string get_today_date(bool ret_recursive, int ydiff=0, int mdiff=0, int ddiff=0);


class LogInfo 
{
public:
  LogInfo(int log_level, const std::string &log_root, 
          std::string file_name = "", bool ltest = false):
  log_level(log_level),
  log_root(log_root),
  init_suffix(""),
  log_dir_today("") {
    log_dir_today = get_log_path();
    img_count_=0;
    local_test = ltest;
    auto logger = spdlog::basic_logger_mt("logger", boost::filesystem::path(log_dir_today).append(file_name + "_" + get_today_date(false) + ".txt").string());
    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::info);

    parse_log_level(log_level);

  };
  ~LogInfo(){ }

  int log_level;
  std::string log_root;               // 日志根目录
  std::string log_dir_today;          // 当日日志目录

  int img_count_=0;
  bool local_test;

  bool log_nothing;                   // 0
  bool log_level_1;                 // 1 
  bool log_level_2;                  // 2
  bool log_level_3;                  // 4 
  bool log_level_4;                   // 8 
  bool log_level_5;                   // 16 

  std::string model_dir;
  std::string init_suffix;
  
private:
  std::mutex mutex_;

public:
void parse_log_level(int log_lvl) {  
  this->log_level_1=false;
  this->log_level_2=false;
  this->log_level_3=false;
  this->log_level_4=false;
  this->log_level_5=false;
  
  if (log_lvl >= 16){this->log_level_5=true; log_lvl -= 16;}
  if (log_lvl >= 8){this->log_level_4=true; log_lvl -= 8;}
  if (log_lvl >= 4){this->log_level_3=true; log_lvl -= 4;}
  if (log_lvl >= 2){this->log_level_2=true; log_lvl -= 2;}
  if (log_lvl >= 1){this->log_level_1=true; log_lvl -= 1;}
  spdlog::get("logger")->info("log_level_5: {}\n", log_level_5);
  spdlog::get("logger")->info("log_level_4: {}\n", log_level_4);
  spdlog::get("logger")->info("log_level_3: {}\n", log_level_3);
  spdlog::get("logger")->info("log_level_2: {}\n", log_level_2);
  spdlog::get("logger")->info("log_level_1: {}\n\n", log_level_1);

}

std::string get_log_path(std::string suffix=std::string()) {
  std::lock_guard<std::mutex> guard(mutex_);
  std::string stem = get_today_date(true);
  boost::filesystem::path ret_path = boost::filesystem::path(this->log_root).append(stem).append(init_suffix).append(suffix);
  if (!boost::filesystem::exists(ret_path)) { boost::filesystem::create_directories(ret_path);}
    if (log_dir_today != ret_path.string()) { log_dir_today = ret_path.string();}
  return ret_path.string();
}

int counter() {
  std::lock_guard<std::mutex> guard(mutex_);
  return img_count_++;
}

};

