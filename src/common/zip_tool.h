#pragma once
#define ZLIB_WINAPI
#include <zlib.h>
#include "base_util/utils.h"
#include "common/base_struct.h"
#include "zip/unzip.h"

#include <iostream>
#include <string>
 
namespace common {

int unzip_current_file(unzFile uf, std::string destFolder);
// 解压zip包函数
int unzip_file(std::string zip_path, std::string unzip_dir);

}   // namespace common
