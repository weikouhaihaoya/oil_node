#include "local_tools.h"

namespace func {

    int string_to_struct(const std::string& str, ConfigInfo& cfg_ifo, bool read_file=true){
        
        // mqttInfo
        if (root.isMember("mqttInfo")) { cfg_ifo.mqtt_ifo = common::parse_mqttInfo(root["mqttInfo"]); }

        return 0;
    }
    
    int struct_to_string(ConfigInfo& cfg_ifo, std::string& str, bool write_file=true){
        
        // mqttInfo
        if (cfg_ifo.mqtt_ifo.ok==0) root["mqttInfo"] = common::export_mqttInfo(cfg_ifo.mqtt_ifo);

        return 0;
    }

}