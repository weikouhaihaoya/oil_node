#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <opencv2/freetype.hpp>
#include "base_util/utils.h"
#include "node.base/total_struct.h"
#include "common/common_tool.h"
#include "common/mqtt_tool.h"
#include"common/base_struct.h"
#include"common/zip_tool.h"
//#include"node.rk3568/oil_node.h"
//using namespace rk3568;
using namespace common;
namespace node {

    using namespace httplib;

    int string_to_struct(const std::string& str, NodeConfigInfo& cfg_ifo, bool read_file=true);
    int struct_to_string(NodeConfigInfo& cfg_ifo, std::string& str, bool write_file=true);
    int update_app(std::string version, std::string updateUrl, MonitorAppTask task, LogInfo *log_ifo=nullptr);
}   // namespace cb
