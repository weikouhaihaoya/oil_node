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

#include "base_util/utils.h"
#include "mcenter/total_struct.h"
#include "mcenter/local_tool.h"

    namespace center {

    class MCenter
    {
        public:
            MCenter(center::ConfigInfo* cfg_info, LogInfo* log_info);
            ~MCenter();

        public:
            int node_event_mqtt(std::string body, std::string& ret_str);
        
    };

    }