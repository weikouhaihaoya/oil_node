#pragma once
#include <string>
#include <vector>
#include <set>

#include "base_util/log_lite.h"

// 硬件校验方式
enum CheckDevice
{
  check_jetson = 1,
  check_x86_win32 = 2,
  check_aarch64 = 3,
  check_x86_linux = 4,
  check_orangepi = 5,
  check_orangepi3 = 6
};

// 硬盘信息
struct DiskInfo {
  DiskInfo():total(0),used(0),free(0),percent(0) {}
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

