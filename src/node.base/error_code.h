#pragma once
#include "base_util/error_code.h"

#define CUR_VERSION "2.3.2"

#define APP_NAME "algo_node"
#define CFG_NODE_PATH "./cfg.node.json"
#define TOTAL_MDL_CFG "cfg_total_mdl.json"
#define ALARM_DATA_DIR "data"

#define API_node_SERVICE_PORT 9980
#define API_aiball_SET_IP "/api/"
#define API_aiball_restart "/api/control/restart"
#define API_aiball_heart "/api/control/heart"
#define API_aiball_stop "/api/control/stop"
#define API_aiball_version "/api/getVersion"
#define API_aiball_getDeviceBasicInfo "/api/System/getDeviceBasicInfo"
#define API_aiball_MediaPreview "/api/Media/Preview"
#define API_aiball_MediaPTZ "/api/Media/PTZ"
#define API_aiball_MediaPlayback "/api/Media/Playback"
#define API_gb28181_server "/api/GB28181/server"
#define API_gb28181_platform "/api/GB28181/platform"
#define API_aiball_GB28181Event "/api/Media/GB28181Event"
#define API_aiball_control_newTask "/api/control/newTask"
#define API_aiball_control_getCameraCfg "/api/control/getCameraCfg"
#define API_node_algo_get_alarm_data "/api/algo/queryAlarmData"
#define API_node_algo_get_algotype "/api/algo/queryAlgoType"
#define API_node_subCenterEvent "/api/node/subCenterEvent"

#define API_ainvr_SERVICE_PORT 9982
#define API_device_get_device_all "/api/device/getDeviceAll"
#define API_device_add_device "/api/device/addDevice"
#define API_device_del_device "/api/device/delDevice"
#define API_device_get_channel_all "/api/device/getChannelAll"
#define API_device_get_channel_algo "/api/channel/getChannelAlgo"
#define API_device_set_channel_algo "/api/channel/setChannelAlgo"
#define API_device_get_channel_image "/api/channel/getChannelImg"
#define API_camera_get_support_algo "/api/camera/getSupportAlgo"
#define API_camera_get_max_play_num "/api/camera/getMaxPlayNum"

#define API_model_get_model_list "/api/model/list"

#define API_media_get_stream_url "/api/media/getStreamUrl"

#define API_ainvr_auth "/api/ainvr/auth"

static MyError SDKQueryAlarmDataERROR(4001, "未查询到对应的报警数据"); 

static MyError QueryDeviceChannelIdDataERROR(4100, "未查询到对应的设备、通道ID数据");

static MyError QueryCameraIdImageERROR(4110, "无法获取图片"); 
static MyError QueryImageSaveERROR(4111, "获取图片失败"); 
static MyError get_stream_url_ERROR(4113, "获取视频流地址失败"); 
static MyError playAlgo_value_ERROR(4116, "获取算法视频流参数错误"); 

static MyError CameraIdDataNotAddToTaskERROR(5100, "对应的相机ID数据未添加到任务"); 

static MyError API_MediaPreview_ERROR(5002, "预览接口处理失败"); 
static MyError API_MediaPTZ_ERROR(5003, "云台控制接口处理失败"); 
static MyError API_Control_NewTask_ERROR(5006, "执行新加任务失败"); 

static MyError DEVICE_IP_CANOT_ACCESS_ERROR(5301, "设备IP地址不可达"); 
static MyError DEVICE_ID_NOT_EXIST_ERROR(5302, "设备ID不存在"); 
static MyError DEVICE_ID_OR_CHANNEL_ID_NOT_EXIST_ERROR(5304, "设备ID或通道ID不存在"); 
static MyError DEVICE_transportType_ERROR(5306, "设备transportType参数错误"); 
static MyError DEVICE_del_failed_by_task_ERROR(5308, "设备删除失败，请先清空算法配置"); 

static MyError TASK_RESTART_ERROR(5402, "重启任务时失败"); 

static MyError MODEL_ID_NOT_EXIST_ERROR(5402, "模型ID不存在"); 
