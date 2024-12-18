#pragma once
#include "base_util/error_code.h"
#include "node.base/error_code.h"

#define CUR_VERSION_RK "1.1.0"
static MyError CAMERA_ID_NOT_EXIST(4000, "相机id不存在"); 
static MyError CAN_NOT_OPEN_VIDEO_STREAM(4002,"不能打开流地址");
static MyError PICTURE_IS_EMPTY(4003,"抓取到的图片为空");
static MyError MODBUSIDX_ERROR(4004,"modbus设备通道id错误");
static MyError MODBUS_TYPE_ERROR(4005,"modbus设备类型错误");
static MyError MODBUS_ADD_ERROR(4006,"modbus设备地址错误");
static MyError MODBUS_READNUMBERS_ERROR(4007,"modbus读取寄存器个数错误");
static MyError MODBUS_Interval_time_ERROR(4008,"modbus设备数据采集间隔错误");
static MyError MODBUS_BOUD_RATE_ERROR(4009,"modbus设备波特率错误");
static MyError MODBUS_DATA_BIT_ERROR(4010,"modbus设备数据位错误");
static MyError MODBUS_PARITY_ERROR(4011,"modbus设备奇偶校验位错误");
static MyError MODBUS_STOP_BIT_ERROR(4012,"modbus设备停止位错误");
static MyError DEVICE_REBOOT_ERROR(4013,"设备重启错误");
static MyError DEVICE_ID_ERROR(4014,"继电器设备id错误");
static MyError DEVICE_GPIO_ERROR(4015,"输入的gpio错误");
static MyError MOD_ID_ERROR(4016,"modbus设备id错误");
static MyError CAM_ID_ERROR(4017,"相机id错误");
static MyError CAM_EMPTY_ERROR(4018,"相机配置为空");
static MyError DEVICE_EMPTY_ERROR(4019,"继电器配置为空");
static MyError MDB_EMPTY_ERROR(4020,"modbus设备为空");
static MyError NET_EMPTY_ERROR(4021,"网络检测点位为空");
static MyError NET_IP_ERROR(4022,"网络ip错误");
namespace node {
        
} //namespace node