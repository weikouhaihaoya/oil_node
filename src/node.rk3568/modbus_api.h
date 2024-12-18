#pragma once
#include<iostream>
#include <string>
#include <vector>
#include <set>
#include<map>
#include <modbus/modbus.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include<common/base_struct.h>
#include<cmath>
#include "node.base/local_tool.h"
#include "node.rk3568/total_struct.h"
#include "base_util/utils.h"
#include<utility>
//#include"node.rk3568/oil_node.h"
using namespace std;
// #include "base_util/utils.h"
// #include "common/total_struct.h"

namespace common {

#define modbus_set_slave_error 1550
#define modbus_connect_error 1551
#define modbus_read_error 1552
#define modbus_not_exists_error 1553
#define modbus_path_0 "/dev/ttyCH9344USB0"
#define modbus_path_1 "/dev/ttyCH9344USB1"
#define modbus_path_2 "/dev/ttyCH9344USB2"
#define modbus_path_3 "/dev/ttyCH9344USB3"
#define modbus_path_4 "/dev/ttyCH9344USB4"
#define modbus_path_5 "/dev/ttyCH9344USB5"
#define modbus_path_6 "/dev/ttyCH9344USB6"
#define modbus_path_7 "/dev/ttyCH9344USB7"

#define modbus_485 MODBUS_RTU_RS485
#define modbus_232 MODBUS_RTU_RS232


// struct SerialDataInfo {
//   SerialDataInfo(): ok(-1) {}

//   int ok;
//   std::string device;
//   int baud;
//   std::string parity;
//   int dataBit;
//   int stopBit;
// };

int get_temperature(SerialDataInfo serial_ifo, float& temp);

class ModbusAPI
{

public:
    ModbusAPI();
    ~ModbusAPI();
    
    int send_data_tcp(SerialDataInfo &md_ifo, uint16_t value, int type);
    int read_data_rtu(std::string deviceId);
    void stop();
    void rtu_listen();
    void tcp_listen();
    void send_rec_mode(int);
    void send_rec_one_step(int);
    void run() ;
    vector<pair<std::string,vector<uint16_t> > > data;

private :
    void receive_modbus_data();
    void receive_rec_data(int rec_idx, int ret, int value);
    void receive_cur_cam_idx(int cam_idx);

private:
    int init_modbus_rtu();
    int init_modbus_tcp();
    void run_listen();


private:
    bool stop_status;

    int init_modbus_rtu_status;
    int init_modbus_tcp_status;
    int rtu_rc;
    uint8_t *rtu_query;
    int tcp_rc;
    uint8_t *tcp_query;
    bool md_ifo_receive_flag;

    modbus_t *modbus_rtu;
    modbus_t *modbus_tcp;
    modbus_mapping_t * mb_mapping;
    int server_socket;
    int socket_fd;
    int modbus_socket;
public:
    struct modbus_config{
        int Baud_rate;
        char Parity;
        int Data_bit;
        int Stop_bit;
        //int mod_pos;
        int readNumbers;
        std::string mod_ID;
        std::string modbus_path; 
        int modbus_mode;
        int mod_add;
        //std::string deviceId;
    };


    //OilNode* node;
    //std::map<string,int> mod_map;
    //node::NodeConfigInfo* cfg_ifo;
    std::vector<modbus_config> mdb_config;
};

}   // namespace smartbox
