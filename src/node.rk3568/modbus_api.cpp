#include "modbus_api.h"

#include <fstream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <thread>

#if defined(_WIN32)
#include <winsock.h>
//#include <winsock2.h>
#include <ws2tcpip.h>
//#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace common {

ModbusAPI::ModbusAPI()
{

    modbus_rtu = nullptr;
    modbus_tcp = nullptr;
    mb_mapping = nullptr;

    init_modbus_rtu_status  = -1;
    init_modbus_tcp_status  = -1;
    stop_status = false;
    server_socket = -1;
    socket_fd = -1;
    modbus_socket = -1;

    tcp_rc = -1;
    rtu_rc = -1;
    rtu_query = new uint8_t[MODBUS_TCP_MAX_ADU_LENGTH];
    memset(rtu_query,0,sizeof MODBUS_TCP_MAX_ADU_LENGTH);
    tcp_query = new uint8_t[MODBUS_TCP_MAX_ADU_LENGTH];
    memset(tcp_query,0,sizeof MODBUS_TCP_MAX_ADU_LENGTH);

    md_ifo_receive_flag = false;

}

ModbusAPI::~ModbusAPI() {
}

int ModbusAPI::send_data_tcp(SerialDataInfo &md_ifo, uint16_t value, int type) {

//     //创建modbus对象
//     modbus_t  *modbus;
//     //建立tcp连接
//     modbus = modbus_new_tcp(md_ifo.tcp_svr_ip.c_str(), md_ifo.tcp_svr_port);
//     //设置slave
//     int  ret  =  modbus_set_slave(modbus, md_ifo.tcp_svr_id);//错误返回-1，正确返回0；
//     if (ret != 0){return modbus_set_slave_error;}
//     //进行连接
//     ret = modbus_connect(modbus);//错误返回-1，正确返回0；
//     if (ret != 0){ return modbus_connect_error;}

//     //设置响应延迟，第三个参数是最迟的时间，1000为1ms不是1s；
//     ret = modbus_set_response_timeout(modbus,0,900000);
//     if (ret != 0){ return modbus_set_response_timeout_error;}

//     //    std::string str_result("12700");
// //    uint16_t tab_reg[20] = { 0 };
//     //    for (size_t i = 0; i < str_result.size(); i++)
//     //    {
//     //        tab_reg[i] = str_result[i];
//     //    }

//     //读数据
// //    ret  = modbus_read_registers(modbus, 0, 4, tab_reg);//读取的数据存储在容器中，错误返回-1
// //    printf("ret:    tab_reg: %d %d %d %d %d %d %d %d\n", ret, tab_reg[0], tab_reg[1], tab_reg[2], tab_reg[3], tab_reg[4], tab_reg[5], tab_reg[6], tab_reg[7]);

//     //写数据
//     // ret = modbus_write_registers(modbus, 0x00, 20, tab_reg); //将容器的数据依次写入寄存器
// //    ret = modbus_write_register(modbus, 3, 167);

//     // ret = modbus_write_register(modbus, md_ifo.meter_addr, value);

// //    ret  = modbus_read_registers(modbus, 0, 4, tab_reg);//读取的数据存储在容器中，错误返回-1
// //    printf("ret:    tab_reg: %d %d %d %d %d %d %d %d\n", ret, tab_reg[0], tab_reg[1], tab_reg[2], tab_reg[3], tab_reg[4], tab_reg[5], tab_reg[6], tab_reg[7]);

//     modbus_close(modbus);
//     modbus_free(modbus);
    return 0;
}

int ModbusAPI::read_data_rtu(std::string mod_ID) {
    // if(device_name==""){
    //     return 1;
    // }

    // char check_type = 'N';  // 'O' 'E'
    int key=0;
    for(int i=0;i<mdb_config.size();i++){
        if(mdb_config[i].mod_ID==mod_ID){
            key=i;
            break;
        }
    } 
    std::string md_path = mdb_config[key].modbus_path;
    //std::cout<<md_path<<std::endl;
    //std::string md_path = mdb_con[mod_map[device_name]].modbus_path;
     //if (!bf::exists(md_path)) { return modbus_not_exists_error;}

    modbus_t *modbus = modbus_new_rtu(md_path.c_str(), mdb_config[key].Baud_rate, mdb_config[key].Parity, mdb_config[key].Data_bit, mdb_config[key].Stop_bit);
    // modbus_t *modbus = modbus_new_rtu(md_ifo.rtu_com.c_str(), md_ifo.rtu_baud,
    //                                   check_type, md_ifo.rtu_data_bit,
    //                                   md_ifo.rtu_stop_bit);        //open port


    int ret = modbus_rtu_set_serial_mode(modbus, mdb_config[key].modbus_mode );
    // std::cout << "modbus_rtu_set_serial_mode ret: " << ret << std::endl;
    // int ret = modbus_rtu_set_serial_mode(modbus, md_ifo.rtu_232_or_485 );  // MODBUS_RTU_RS232  设置串口模式
    // if (ret != 0){return modbus_rtu_set_serial_mode_error;}
   // printf("配置串口:%d\n",ret);
    
    ret = modbus_set_slave(modbus, 1);//set slave address
    // std::cout << "modbus_set_slave ret: " << ret << std::endl;
    // ret = modbus_set_slave(modbus, md_ifo.rtu_slave_id);//set slave address
    //printf("配置从机地址：%d\n",ret);
    if (ret != 0){ return modbus_set_slave_error;}
    // if(ret!=0){
    //     return ret;
    // }


    ret = modbus_connect(modbus);
    // std::cout << "modbus_connect ret: " << ret << std::endl;
    //printf("连接：%d\n",ret);
    if (ret != 0){ return modbus_connect_error;}
    // if(ret!=0){
    //     return ret;
    // }

    timeval t = {1,0};
    modbus_set_response_timeout(modbus, &t);
    // if (ret != 0){ return modbus_set_response_timeout_error;}

    // 读数据
    uint16_t reg[20] = { 0 };

    ret = modbus_read_registers(modbus, mdb_config[key].mod_add, mdb_config[key].readNumbers, reg);//读取的数据存储在容器中，错误返回-1
    //data[mod_map[device_name]]=((double)reg[0])/10;
    //data.push_back(make_pair(mod_ID,((double)reg[0])/10));
    vector<uint16_t> v;
    for(int i=0;i<mdb_config[key].readNumbers;i++){
        v.push_back(reg[i]);
    }
    printf("数据：%d %d",reg[0],reg[1]);
    bool istrue=0;
    for(int i=0;i<data.size();i++){
        if(data[i].first==mod_ID){
            //如果原先已经有数据，则直接覆盖
            data[i].second=v;
            istrue=1;
        }
    }
    if(!istrue){
        data.push_back(make_pair(mod_ID,v));
    }
    //printf("%s数据:%d\n",device_name,data[mod_map[device_name]]);
    // std::cout << "modbus_read_registers ret: " << ret << std::endl;
    //printf("ret:  %d  tab_reg: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", ret, tab_reg[0], tab_reg[1], tab_reg[2], tab_reg[3], tab_reg[4], tab_reg[5], tab_reg[6], tab_reg[7], tab_reg[8], tab_reg[9], tab_reg[10], tab_reg[11], tab_reg[12], tab_reg[13], tab_reg[14], tab_reg[15], tab_reg[16], tab_reg[17], tab_reg[18], tab_reg[19]);
   // if (ret < 0){ return modbus_read_error;}
    // if(ret!=0){
    //     return ret;
    // }

    // ret = modbus_write_register(modbus, md_ifo.meter_addr, value);
    // std::cout << "tab_reg: " << uint16_t(tab_reg[0]) << " "  << uint16_t(tab_reg[1]) << " "  << uint16_t(tab_reg[2]) << " "  << uint16_t(tab_reg[3]) << " "  << uint16_t(tab_reg[4]) << " "  << uint16_t(tab_reg[5]) << " " << uint16_t(tab_reg[6]) << std::endl;
    // value = int(static_cast<int32_t>(tab_reg[5])) / 1000;
    // std::cout << "value: " << value << std::endl;

    modbus_close(modbus);
    modbus_free(modbus);
    return 0;
}

int get_temperature(SerialDataInfo modbusInfo, float& temp){
    // if (modbusInfo.device.empty() || !bf::exists(modbusInfo.device)) { return modbus_not_exists_error;}
    timeval t = {1,0};
    uint16_t tab_reg[20] = { 0 };

    modbus_t *modbus = modbus_new_rtu(modbusInfo.device.c_str(), modbusInfo.baud, modbusInfo.parity[0], modbusInfo.dataBit, modbusInfo.stopBit);
    int ret = modbus_rtu_set_serial_mode(modbus, MODBUS_RTU_RS485 );
    // std::cout << "modbus_rtu_set_serial_mode ret: " << ret << std::endl;

    ret = modbus_set_slave(modbus, 1);//set slave address
    // std::cout << "modbus_set_slave ret: " << ret << std::endl;
    if (ret != 0){ ret = modbus_set_slave_error; goto free;}

    ret = modbus_connect(modbus);
    // std::cout << "modbus_connect ret: " << ret << std::endl;
    if (ret != 0){ ret = modbus_connect_error; goto free;}

    modbus_set_response_timeout(modbus, &t);

    // 读数据
    ret = modbus_read_registers(modbus, 0, 2, tab_reg);//读取的数据存储在容器中，错误返回-1
    // std::cout << "modbus_read_registers ret: " << ret << std::endl;
    // printf("ret:  %d  tab_reg: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", ret, tab_reg[0], tab_reg[1], tab_reg[2], tab_reg[3], tab_reg[4], tab_reg[5], tab_reg[6], tab_reg[7], tab_reg[8], tab_reg[9], tab_reg[10], tab_reg[11], tab_reg[12], tab_reg[13], tab_reg[14], tab_reg[15], tab_reg[16], tab_reg[17], tab_reg[18], tab_reg[19]);
    if (ret < 0){ ret = modbus_read_error; goto free;}
    temp = int(tab_reg[0]) * 1.0 / 10;
    // std::cout << "temp: " << temp << std::endl;

free:
    modbus_close(modbus);
    modbus_free(modbus);
    return ret;
}

}   // namespace smartbox
