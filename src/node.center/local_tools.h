#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "common/total_struct.h"
#include "total_struct.h"

namespace func {

int string_to_struct(const std::string& str, ConfigInfo& cfg_ifo, bool read_file=true);
int struct_to_string(ConfigInfo& cfg_ifo, std::string& str, bool write_file=true);

}//namespace func
