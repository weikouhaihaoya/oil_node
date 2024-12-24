#pragma once
#include <string>
#include <vector>
#include <set>

#include "base_util/log_lite.h"

namespace common {


enum CFGFileUpdateFlag {
  CFGFileNoUpdate,
  CFGFileUpdateCore,
  CFGFileUpdateOther,
};

enum PostDataType {
  onlyPostText=1,  
  onlyPostImage=2,  
  onlyPostVideo=4,
};

enum RunningStatus {
  running_stoped = -1,    //  运行完成
  running_notify = 0,     //  通知运行
  running_running = 1,    //  正在运行
};

enum CameraStatus {
  CameraClosed = -1,    //  -1:未打开相机
  CameraOpened = 0,     //  0: 相机已打开
  CameraGrabing = 1,    //  1: 正在抓图
};

enum CameraType {
  CameraNone,
  CameraPTZ,        // 球机
  CameraIC,         // 枪机
  CameraThermal,    // 测温相机
  CameraRobot,      // 工业相机
};

struct AiNvrInfo {
  AiNvrInfo(): ok(-1) {}

  int ok;
  std::string ainvrServer;
  std::string ainvrAuthApi;
};

struct AppMonitorInfo {
  AppMonitorInfo(): ok(-1) {}

  int ok;
  std::string monitorServer;
  std::string peerRestart;
};

struct HttpServerInfo {
  HttpServerInfo(): ok(-1) {}

  int ok;
  std::string httpServer;

  std::string uploadHttpHeart;      // 上传http心跳
  std::string uploadFile;           // 通用上传文件接口
  std::string uploadCameraImg;      // 上传监控点图片
  std::string uploadAlarmFast;      // 快速上传报警数据
  std::string uploadAlarmSlow;      // 上传报警数据附加数据: 视频 图片

  std::string queryAlgorithm;       // 查询算法
  std::string queryVersion;         // 查询版本

  std::string checkDevice;
  std::string queryDevice;
  std::string imageDetect;
  std::string checkRunning;
  std::string updateInitInfo;
  std::string queryInitInfo;
  std::string queryMarkerInfo;
};

struct GB28181SRS {
  GB28181SRS(): ok(-1) {}
  GB28181SRS(const GB28181SRS& cib) {
    *this = operator=(cib);
  }
  GB28181SRS& operator=(const GB28181SRS& ci) {
    if (this == &ci) { return *this; }
    rtspAddress = ci.rtspAddress;
    httpServer = ci.httpServer;
    deviceId = ci.deviceId;
    queryDevice = ci.queryDevice;
    queryChannel = ci.queryChannel;
    playStart = ci.playStart;
    return *this;
  }
  bool operator==(const GB28181SRS& ci) const {
    if (this == &ci) { return true; }
    // spdlog::get("logger")->info("rtspAddress:{}, ci.rtspAddress:{}", rtspAddress, ci.rtspAddress);
    // spdlog::get("logger")->info("httpServer:{}, ci.httpServer:{}", httpServer, ci.httpServer);
    // spdlog::get("logger")->info("deviceId:{}, ci.deviceId:{}", deviceId, ci.deviceId);
    // spdlog::get("logger")->info("queryDevice:{}, ci.queryDevice:{}", queryDevice, ci.queryDevice);
    // spdlog::get("logger")->info("queryChannel:{}, ci.queryChannel:{}", queryChannel, ci.queryChannel);
    // spdlog::get("logger")->info("playStart:{}, ci.playStart:{}", playStart, ci.playStart);

    return rtspAddress == ci.rtspAddress && httpServer == ci.httpServer && \
          deviceId == ci.deviceId && queryDevice == ci.queryDevice && \
          queryChannel == ci.queryChannel && playStart == ci.playStart;
  }
  int ok;
  std::string rtspAddress;              // 配置流地址的时候优先使用
  std::string httpServer;
  std::string deviceId;
  std::string queryDevice;
  std::string queryChannel;
  std::string playStart;
};

struct MqttCfgData {
  MqttCfgData() {}
  MqttCfgData(const MqttCfgData& ci) {
    *this = operator=(ci);
  }
  MqttCfgData& operator=(const MqttCfgData& ci) {
    if (this == &ci) { return *this; }
    this->user = ci.user;
    this->password = ci.password;
    this->host = ci.host;
    this->port = ci.port;
    return *this;
  }

	std::string user;
	std::string password;
	std::string host;
	int port;
};

struct MqttInfo: public MqttCfgData {
  MqttInfo(): MqttCfgData(), ok(-1) {}
  MqttInfo(const MqttInfo& ci) {
    *this = operator=(ci);
  }
  MqttInfo& operator=(const MqttInfo& ci) {
    if (this == &ci) { return *this; }
    MqttCfgData::operator=(ci);
    this->ok = ci.ok;
    this->nodePubTopic = ci.nodePubTopic;
    this->nodeSubTopic = ci.nodeSubTopic;
    this->addDevice = ci.addDevice;
    this->addDeviceResult = ci.addDeviceResult;
    this->updateDeviceStatus = ci.updateDeviceStatus;
    this->playStart = ci.playStart;
    this->playResult = ci.playResult;
    return *this;
  }

  int ok;

  std::string nodePubTopic;
  std::string nodeSubTopic;
  std::string addDevice;
  std::string addDeviceResult;
  std::string updateDeviceStatus;
  std::string playStart;
  std::string playResult;
};

struct CameraInfoBase {
  CameraInfoBase() {
    gb28181 = GB28181SRS();
    cameraStatus = -1;      // common::CameraClosed
  }
  CameraInfoBase(const CameraInfoBase& cib) {
    *this = operator=(cib);
  }
  CameraInfoBase& operator=(const CameraInfoBase& ci) {
    if (this == &ci) { return *this; }
    cameraAlias = ci.cameraAlias;
    cameraType = ci.cameraType;
    cameraBrand = ci.cameraBrand;
    cameraId = ci.cameraId;
    cameraIdx = ci.cameraIdx;
    passwd = ci.passwd;
    user = ci.user;
    cameraIp = ci.cameraIp;
    collectImg = ci.collectImg;
    rtspAddress = ci.rtspAddress;
    streamAddress = ci.streamAddress;
    streamTime = ci.streamTime;
    gb28181 = ci.gb28181;
    cameraStatus = int(ci.cameraStatus);
    update_local_time = ci.update_local_time;
    return *this;
  }
  bool operator==(const CameraInfoBase& ci) const {
    if (this == &ci) { return true; }
    
    // spdlog::get("logger")->info("cameraType:{}, ci.cameraType:{}", cameraType, ci.cameraType);
    // spdlog::get("logger")->info("cameraBrand:{}, ci.cameraBrand:{}", cameraBrand, ci.cameraBrand);
    // spdlog::get("logger")->info("cameraId:{}, ci.cameraId:{}", cameraId, ci.cameraId);
    // spdlog::get("logger")->info("passwd:{}, ci.passwd:{}", passwd, ci.passwd);
    // spdlog::get("logger")->info("user:{}, ci.user:{}", user, ci.user);
    // spdlog::get("logger")->info("cameraIp:{}, ci.cameraIp:{}", cameraIp, ci.cameraIp);
    // spdlog::get("logger")->info("rtspAddress:{}, ci.rtspAddress:{}", rtspAddress, ci.rtspAddress);
    // spdlog::get("logger")->info("streamAddress:{}, ci.streamAddress:{}", streamAddress, ci.streamAddress);
    // spdlog::get("logger")->info("streamTime:{}, ci.streamTime:{}", streamTime, ci.streamTime);

    // 仅核心信息需要判断是否相等
    return cameraAlias == ci.cameraAlias && cameraType == ci.cameraType && cameraBrand == ci.cameraBrand && \
          cameraId == ci.cameraId && cameraIdx == ci.cameraIdx && passwd == ci.passwd && user == ci.user && \
          cameraIp == ci.cameraIp && gb28181 == ci.gb28181 && update_local_time == ci.update_local_time;
  }
  std::string cameraAlias;
  std::string cameraType;                 // 相机类型 IC PTZ THERMAL USB GB28181
  std::string cameraBrand;
  std::string cameraId;
  int         cameraIdx;
  std::string passwd;
  std::string user;
  std::string cameraIp;
  std::string collectImg;                 // 是否采集图片 "1"  "0"
  std::string rtspAddress;                    // 拉流地址
  std::string streamAddress;              // 推流地址
	std::string streamTime;                 // 开始推流时间 不保存在配置文件里
  // NOTE:不能放在此处，没有cv命名空间
  // cv::Rect rec_region;                        // 识别区域 仅用作与服务器端进行比较
  
  GB28181SRS gb28181;
  std::atomic_int cameraStatus;
  std::string update_local_time;
};

struct NVRInfo {
  NVRInfo(): ok(-1), NVRIp(""), NVRChannel("") {}

  int ok;
  std::string NVRIp;
  std::string NVRChannel;
  std::string NVRBrand;
  std::string passwd;
  std::string user;
};

struct CameraGroupBase {
  CameraGroupBase() {
    nvr_ifo = NVRInfo();
  }
  CameraGroupBase(const CameraGroupBase& cgb) {
    *this = operator=(cgb);
  }
  CameraGroupBase& operator=(const CameraGroupBase& ci) {
    if (this == &ci) { return *this; }
    this->nvr_ifo = ci.nvr_ifo;
    return *this;
  }

  NVRInfo nvr_ifo;
};

struct PostInfoBase {
  PostInfoBase(): start_time(0), end_time(0), time(0), try_cnt(0) {}
  PostInfoBase( int64_t start_time, int64_t end_time, int64_t time, std::string algo_type, 
            int alarm, std::string img_path, std::string alarm_info, int try_cnt):
            start_time(start_time), end_time(end_time), time(time),
            algo_type(algo_type), alarm(alarm), img_path(img_path),
            alarm_info(alarm_info), try_cnt(try_cnt){ }

  PostInfoBase( int64_t start_time, int64_t end_time, int64_t time, std::string algo_type, 
            int alarm):
            PostInfoBase(start_time, end_time, time, algo_type, alarm, "", "", 0) { }
  
  PostInfoBase( int64_t start_time, int64_t end_time, int64_t time, std::string algo_type, 
            int alarm, std::string img_path):
            PostInfoBase(start_time, end_time, time, algo_type, alarm, img_path, "", 0) { }
  
  PostInfoBase( int64_t start_time, int64_t end_time, int64_t time, std::string algo_type, 
            int alarm, std::string img_path, std::string alarm_info):
            PostInfoBase(start_time, end_time, time, algo_type, alarm, img_path, alarm_info, 0) { }

  PostInfoBase(const PostInfoBase& pib) {
    *this = operator=(pib);
  }
  PostInfoBase& operator=(const PostInfoBase& pi) {
    if (this == &pi) { return *this; }
    start_time = pi.start_time;
    end_time = pi.end_time;
    time = pi.time;
    algo_type = pi.algo_type;
    alarm = pi.alarm;
    img_path = pi.img_path;
    alarm_info = pi.alarm_info;
    try_cnt = pi.try_cnt;
    return *this;
  }

  int64_t       start_time;
  int64_t       end_time;
  int64_t       time;
  std::string algo_type;
  int         alarm;
  std::string img_path;
  std::string alarm_info;
  int try_cnt;

};


struct MediaKitAPI {
  std::string getRtpInfo;
  std::string openRtpServer;
  std::string closeRtpServer;
  std::string getMediaList;
  std::string startSendRtp;
  std::string stopSendRtp;
  std::string addCamera;
  std::string addStreamProxy;
  std::string getVideo;
  std::string cameraProxy;
  std::string closeStreams;
  std::string kickSessions;
  std::string addDeleteStrategy;
};

struct DeleteStrategy {
  int mainStreamDay;
  int mainStreamHour;
  int subStreamDay;
  int subStreamHour;
};

struct MediaKitInfo {
  MediaKitInfo(): ok(-1) {}

  int ok;
  std::string secret;
  std::vector<std::string> mediaKitServers;
  std::string mediaKitServer;
  std::string mediaKitClient;
  MediaKitAPI mediaKitAPI;
  DeleteStrategy deleteStrategy;
};

struct SerialDataInfo {
  SerialDataInfo(): ok(-1) {}

  int ok;
  std::string device;
  int baud;
  std::string parity;
  int dataBit;
  int stopBit;
};

struct PTZControlInfo {
  PTZControlInfo(): p(0), t(0), z(0), move_status(-1), zoom_status(-1) {}
  ~PTZControlInfo() {}
  PTZControlInfo(const PTZControlInfo& ci) {
    *this = operator=(ci);
  }
  PTZControlInfo& operator=(const PTZControlInfo& ci) {
    if (this == &ci) { return *this; }
    this->p = ci.p;
    this->t = ci.t;
    this->z = ci.z;
    this->move_status = ci.move_status;
    this->zoom_status = ci.zoom_status;
    return *this;
  }
  float p;
  float t;
  float z;

  int move_status;    // 1 正在移动， 0 移动完成 
  int zoom_status;    // 1 正在移动， 0 移动完成 
};

struct MonitorAppAPI {
  int ok;
  std::string httpServer;      
  std::string heart;      
  std::string stop;           
  std::string version;           
};

struct MonitorUpdateAPI {
  int ok;
  std::string httpServer;      
  std::string update;      
};
struct dev_con{
    int gpio;
    std::string deviceID;
    int restart_time;

};
struct camera{
  std::string IP;
  std::string passwd;
  std::string user;
  std::string brand;
  int Grab_time;
  std::string cameraID;
};
struct modbus_con{
  modbus_con(){
    env_time=0;
  }
    int modbusIdx;
    std::string modebusType;
    int mod_add;
    int readNumbers;
    int Interval_time;
    int Baud_rate;
    int Data_bit;
    char Parity;
    int Stop_bit;
    std::string mod_ID;
    int64_t env_time;
    //int mod_pos;
};
struct Network{
  Network(){
    net_time=0;
  }
  std::string IP;
  int Interval_time;
  int64_t net_time;
};
struct MonitorAppTask {
  MonitorAppTask() {
    aApi.ok = 0;
    uApi.ok = 0;
    offline_cnt = 0;
  }
  std::string name;      
  std::string startCommand;           
  std::string appRoot;      

  MonitorAppAPI aApi;
  MonitorUpdateAPI uApi;

  int offline_cnt;
  int64_t last_time;
  int64_t start_time;
  int loopTime;                 // 监听间隔

  int maxRunTime;
  int startHour;
  int endHour;
};


struct GBPlatformInfo {
  GBPlatformInfo(): ok(-1) {}
  void init() {
    ok = 0;
    enable = "1";
    ProtocolType= "28181";
    ProtocolVersion= "GB/T28181-2016";
    TransportProtocol= "UDP";
    SIPServerID= "";
    SIPServerIDPrefix= "";
    SIPServerIP= "";
    SIPServerPort= 18066;
    SIPServerPasswd= "";
  }
  GBPlatformInfo(const GBPlatformInfo& ci) {
    *this = operator=(ci);
  }
  GBPlatformInfo& operator=(const GBPlatformInfo& ci) {
    if (this == &ci) { return *this; }
    this->ok = ci.ok;
    this->enable = ci.enable;
    this->ProtocolType = ci.ProtocolType;
    this->ProtocolVersion = ci.ProtocolVersion;
    this->TransportProtocol = ci.TransportProtocol;
    this->SIPServerID = ci.SIPServerID;
    this->SIPServerIDPrefix = ci.SIPServerIDPrefix;
    this->SIPServerIP = ci.SIPServerIP;
    this->SIPServerPort = ci.SIPServerPort;
    this->SIPServerPasswd = ci.SIPServerPasswd;
    return *this;
  }
  bool operator==(const GBPlatformInfo& ci) const {
    if (this == &ci) { return true; }
    return enable == ci.enable && ProtocolType == ci.ProtocolType && ProtocolVersion == ci.ProtocolVersion &&
    TransportProtocol == ci.TransportProtocol && SIPServerID == ci.SIPServerID && SIPServerIDPrefix == ci.SIPServerIDPrefix &&
    SIPServerIP == ci.SIPServerIP && SIPServerPort == ci.SIPServerPort && SIPServerPasswd == ci.SIPServerPasswd;
  }
  bool operator!=(const GBPlatformInfo& ci) const { return !operator==(ci);}

  // 作为国标服务器需要的信息
  int ok;
  std::string enable;
  std::string ProtocolType;
  std::string ProtocolVersion;
  std::string TransportProtocol;
  std::string SIPServerID;
  std::string SIPServerIDPrefix;
  std::string SIPServerIP;
  int         SIPServerPort;
  std::string SIPServerPasswd;
};

struct GBServerInfo {
  GBServerInfo(): ok(-1), RegisterStatus(0) {}
  void init() {
    ok = 0;
    enable = "0";
    Manufacture= "zqdl";
    ProtocolType= "28181";
    ProtocolVersion= "GB/T28181-2016";
    TransportProtocol= "TCP";
    SIPServerID= "";
    SIPServerIDPrefix= "";
    SIPServerIP= "";
    SIPServerPort= 5060;
    SIPClientIP= "";
    SIPClientUser= "";
    SIPClientUserID= "";
    SIPClientUserPasswd= "";
    SIPClientPort= 5060;
    HeartBeatCycle= 60;
    RegisterValidTime= 3600;
    RegisterStatus= 0;
    RegisterInterval= 60;
    MAXHeartBeatTimeoutTimes= 3;
    VideoEncoderID= "";
    AudioEncoderID= "";
  }
  GBServerInfo(const GBServerInfo& ci) {
    *this = operator=(ci);
  }
  GBServerInfo& operator=(const GBServerInfo& ci) {
    if (this == &ci) { return *this; }
    this->ok = ci.ok;
    this->enable = ci.enable;
    this->Manufacture = ci.Manufacture;
    this->ProtocolType = ci.ProtocolType;
    this->ProtocolVersion = ci.ProtocolVersion;
    this->TransportProtocol = ci.TransportProtocol;
    this->SIPServerID = ci.SIPServerID;
    this->SIPServerIDPrefix = ci.SIPServerIDPrefix;
    this->SIPServerIP = ci.SIPServerIP;
    this->SIPServerPort = ci.SIPServerPort;
    this->SIPClientUser = ci.SIPClientUser;
    this->SIPClientUserID = ci.SIPClientUserID;
    this->SIPClientUserPasswd = ci.SIPClientUserPasswd;
    this->SIPClientPort = ci.SIPClientPort;
    this->HeartBeatCycle = ci.HeartBeatCycle;
    this->RegisterValidTime = ci.RegisterValidTime;
    this->RegisterStatus = ci.RegisterStatus;
    this->RegisterInterval = ci.RegisterInterval;
    this->MAXHeartBeatTimeoutTimes = ci.MAXHeartBeatTimeoutTimes;
    this->VideoEncoderID = ci.VideoEncoderID;
    this->AudioEncoderID = ci.AudioEncoderID;
    return *this;
  }
  bool operator==(const GBServerInfo& ci) const {
    if (this == &ci) { return true; }
    return this->ok == ci.ok &&
      this->enable == ci.enable &&
      this->Manufacture == ci.Manufacture &&
      this->ProtocolType == ci.ProtocolType &&
      this->ProtocolVersion == ci.ProtocolVersion &&
      this->TransportProtocol == ci.TransportProtocol &&
      this->SIPServerID == ci.SIPServerID &&
      this->SIPServerIDPrefix == ci.SIPServerIDPrefix &&
      this->SIPServerIP == ci.SIPServerIP &&
      this->SIPServerPort == ci.SIPServerPort &&
      this->SIPClientUser == ci.SIPClientUser &&
      this->SIPClientUserID == ci.SIPClientUserID &&
      this->SIPClientUserPasswd == ci.SIPClientUserPasswd &&
      this->SIPClientPort == ci.SIPClientPort &&
      this->HeartBeatCycle == ci.HeartBeatCycle &&
      this->RegisterValidTime == ci.RegisterValidTime &&
      this->RegisterStatus == ci.RegisterStatus &&
      this->RegisterInterval == ci.RegisterInterval &&
      this->MAXHeartBeatTimeoutTimes == ci.MAXHeartBeatTimeoutTimes &&
      this->VideoEncoderID == ci.VideoEncoderID &&
      this->AudioEncoderID == ci.AudioEncoderID;
  }
  bool operator!=(const GBServerInfo& ci) const { return !operator==(ci);}

  // 作为国标客户端连接国标服务器需要的信息
  int ok;
  std::string enable;
  std::string Manufacture;
  std::string ProtocolType;
  std::string ProtocolVersion;
  std::string TransportProtocol;
  std::string SIPServerID;
  std::string SIPServerIDPrefix;
  std::string SIPServerIP;
  int         SIPServerPort;
  std::string SIPClientIP;
  std::string SIPClientUser;
  std::string SIPClientUserID;
  std::string SIPClientUserPasswd;
  int         SIPClientPort;
  int         HeartBeatCycle;
  int         RegisterValidTime;
  int         RegisterStatus;
  int         RegisterInterval;
  int         MAXHeartBeatTimeoutTimes;
  std::string VideoEncoderID;
  std::string AudioEncoderID;

  std::string RtpIP;
  int         RtpPort;
  std::string RtpProtocol;
  std::string ssrc;

};


struct GBHttpInfo {
  GBHttpInfo(): ok(-1) {}
  GBHttpInfo(const GBHttpInfo& ci) {
    *this = operator=(ci);
  }
  GBHttpInfo& operator=(const GBHttpInfo& ci) {
    if (this == &ci) { return *this; }
    this->ok = ci.ok;
    this->token = ci.token;
    this->httpServer = ci.httpServer;
    this->user = ci.user;
    this->password = ci.password;
    
    this->login = ci.login;
    this->proxySave = ci.proxySave;
    this->queryTransport = ci.queryTransport;
    this->addPlatformAdd = ci.addPlatformAdd;
    this->playStart = ci.playStart;
    this->addPlatformChannelList = ci.addPlatformChannelList;
    this->addPlatformUpdateChannel = ci.addPlatformUpdateChannel;
    this->addPlatformGBStreamList = ci.addPlatformGBStreamList;
    this->addPlatformGBStreamAdd = ci.addPlatformGBStreamAdd;
    this->addPlatformConfig = ci.addPlatformConfig;
    this->addPlatformQuery = ci.addPlatformQuery;
    this->addDevices = ci.addDevices;
    this->delDevices = ci.delDevices;
    this->queryDevices = ci.queryDevices;
    this->queryChannels = ci.queryChannels;
    this->startSendRtp = ci.startSendRtp;
    this->stopSendRtp = ci.stopSendRtp;
    return *this;
  }

  int ok;
  std::string token;
  std::string httpServer;
  std::string user;
  std::string password;

  std::string login;
  std::string proxySave;
  std::string queryTransport;
  std::string addPlatformAdd;
  std::string playStart;
  std::string addPlatformChannelList;
  std::string addPlatformUpdateChannel;
  std::string addPlatformGBStreamList;
  std::string addPlatformGBStreamAdd;
  std::string addPlatformConfig;
  std::string addPlatformQuery;
  std::string addDevices;
  std::string delDevices;
  std::string queryDevices;
  std::string queryChannels;
  std::string startSendRtp;
  std::string stopSendRtp;
};


struct WvpInfo {
  WvpInfo(): ok(-1) {}
  WvpInfo(const WvpInfo& ci) {
    *this = operator=(ci);
  }
  WvpInfo& operator=(const WvpInfo& ci) {
    if (this == &ci) { return *this; }
    this->ok = ci.ok;
    this->token = ci.token;
    this->serverGBId = ci.serverGBId;
    this->serverGBDomain = ci.serverGBDomain;
    this->serverIP = ci.serverIP;
    this->serverPort = ci.serverPort;
    this->serverpassword = ci.serverpassword;
    this->httpServer = ci.httpServer;
    this->user = ci.user;
    this->password = ci.password;
    this->login = ci.login;
    this->proxySave = ci.proxySave;
    this->queryTransport = ci.queryTransport;
    this->addPlatformAdd = ci.addPlatformAdd;
    this->playStart = ci.playStart;
    this->addPlatformChannelList = ci.addPlatformChannelList;
    this->addPlatformUpdateChannel = ci.addPlatformUpdateChannel;
    this->addPlatformGBStreamList = ci.addPlatformGBStreamList;
    this->addPlatformGBStreamAdd = ci.addPlatformGBStreamAdd;
    this->addPlatformConfig = ci.addPlatformConfig;
    this->addPlatformQuery = ci.addPlatformQuery;
    this->queryDevices = ci.queryDevices;
    this->queryChannels = ci.queryChannels;
    return *this;
  }

  int ok;
  std::string token;

  std::string serverGBId;
  std::string serverGBDomain;
  std::string serverIP;
  std::string serverPort;
  std::string serverpassword;
  
  std::string httpServer;
  std::string user;
  std::string password;

  std::string login;
  std::string proxySave;
  std::string queryTransport;
  std::string addPlatformAdd;
  std::string playStart;
  std::string addPlatformChannelList;
  std::string addPlatformUpdateChannel;
  std::string addPlatformGBStreamList;
  std::string addPlatformGBStreamAdd;
  std::string addPlatformConfig;
  std::string addPlatformQuery;
  std::string queryDevices;
  std::string queryChannels;

};

struct GB28181Info {
  GB28181Info() {}
  GB28181Info(const GB28181Info& ci) {
    *this = operator=(ci);
  }
  GB28181Info& operator=(const GB28181Info& ci) {
    if (this == &ci) { return *this; }
    this->plt_ifo = ci.plt_ifo;
    this->svr_ifo = ci.svr_ifo;
    this->http_ifo = ci.http_ifo;
    return *this;
  }

  common::GBPlatformInfo plt_ifo;
  common::GBServerInfo svr_ifo;
  common::GBHttpInfo http_ifo;
  common::WvpInfo wvp_ifo;
};



///通道数据，这个数据需要通过catalog后获取到的，基本上就是相机的一个通道
struct ChannelInfo {
  ChannelInfo(): ok(-1), call_id(-1), channelIdx(-1), onLine(0) {
    last_heart_time = 0; 
    channelImgTime = 0;
  }
  ChannelInfo(std::string channelId, int channelIdx, std::string name, 
    std::string rtspUrl, int onLine): 
    ok(0), channelId(channelId), channelIdx(channelIdx), name(name), 
    rtspUrl(rtspUrl), onLine(onLine)
    {
      last_heart_time = 0;
      channelImgTime = 0;
  }
  ChannelInfo(const ChannelInfo& ci) {
    *this = operator=(ci);
  }
  ChannelInfo& operator=(const ChannelInfo& ci) {
    if (this == &ci) { return *this; }
    this->ok = ci.ok;
    this->rtspUrl = ci.rtspUrl;
    this->channelId = ci.channelId;
    this->channelIdx = ci.channelIdx;
    this->name = ci.name;
    this->manufacturer = ci.manufacturer;
    this->status = ci.status;
    this->onLine = ci.onLine;
    this->last_heart_time = ci.last_heart_time;
    this->channelImgUrl = ci.channelImgUrl;
    this->channelImgTime = ci.channelImgTime;


    this->call_id = ci.call_id;
    this->dialog_id = ci.dialog_id;
    this->ssrc = ci.ssrc;
    this->RtpIP = ci.RtpIP;
    this->RtpPort = ci.RtpPort;
    this->RtpProtocol = ci.RtpProtocol;
    return *this;
  }

  int ok;
  std::string rtspUrl;
  std::string channelId;
  int         channelIdx;               // NVR中需要保存对应通道id, 便于获取视频流
  std::string name;
  int         onLine;                   // 0:离线， 1:在线
  std::string manufacturer;
  int64_t     channelImgTime;            // 用于上传通道图片
  std::string channelImgUrl;            // 用于上传通道图片

  std::string status;
  int64_t     last_heart_time;


  int call_id; //发送invite的call id, 此id不为-1 说明正在发送invite请求，发送invite过程中，不能再次发送invite请求，否则会有问题。
  int dialog_id;
  std::string ssrc; //rtp身份标志，用于invite请求，用来区分多个rtp流。
  std::string RtpIP;
  int RtpPort;
  std::string RtpProtocol;
  uint32_t mReceiveRtpTime;     //接收到rtp数据的时间(用来判断rtp数据接收是否超时了)
  bool mIsCurrentFrameLostPacket;
  uint32_t mLastSequenceNumber; //上一次的rtp序号

};

struct DeviceInfo {
  DeviceInfo(): ok(-1), onLine(0) {}
  DeviceInfo(std::string deviceId, std::string name, 
             std::string channelStartId, int channelCount,
             std::string ip, 
             std::string manufacturer, std::string deviceType,
             std::string deviceAccount, std::string password,
             std::string transportType,
             std::string streamMode): 
    ok(0), onLine(0), deviceId(deviceId), name(name), channelStartId(channelStartId),
    channelCount(channelCount), ip(ip), manufacturer(manufacturer),
    deviceType(deviceType), deviceAccount(deviceAccount),
    password(password), transportType(transportType),
    streamMode(streamMode)
    {

  }
  DeviceInfo(const DeviceInfo& ci) {
    *this = operator=(ci);
  }
  DeviceInfo& operator=(const DeviceInfo& ci) {
    if (this == &ci) { return *this; }
    this->ok = ci.ok;
    this->deviceId = ci.deviceId;
    this->name = ci.name;
    this->channelStartId = ci.channelStartId;
    this->channelCount = ci.channelCount;
    this->ip = ci.ip;
    this->port = ci.port;
    this->manufacturer = ci.manufacturer;
    this->model = ci.model;
    this->deviceType = ci.deviceType;
    this->deviceAccount = ci.deviceAccount;
    this->password = ci.password;
    this->transportType = ci.transportType;
    this->streamMode = ci.streamMode;
    this->onLine = ci.onLine;
    this->last_heart_time = ci.last_heart_time;
    this->channels = ci.channels;
    return *this;
  }

  int ok;
  std::string deviceId;
  std::string name;
  std::string channelStartId;
  int         channelCount;
  std::string ip;
  int         port;
  std::string manufacturer;
  std::string model;
  std::string deviceType;
  std::string deviceAccount;
  std::string password;
  std::string transportType;
  std::string streamMode;
  int         onLine;             // 0:离线， 1:在线
  int64_t     last_heart_time;

  std::vector<ChannelInfo> channels;
};

struct ConfigInfoBase {
  ConfigInfoBase() {
  }
  ConfigInfoBase(const ConfigInfoBase& cib) {
    *this = operator=(cib);
  }
  ConfigInfoBase& operator=(const ConfigInfoBase& cib) {
    if (this == &cib) { return *this; }
    this->runMode = cib.runMode;
    this->nodeName = cib.nodeName;
    this->serialNumber = cib.serialNumber;
    this->nodeArch = cib.nodeArch;
    this->nodeId = cib.nodeId;
    this->nodeIp = cib.nodeIp;
    this->configVersion = cib.configVersion;

    this->modelDir = cib.modelDir;
    this->mediaDir = cib.mediaDir;
    this->historyDir = cib.historyDir;
    this->historyDay = cib.historyDay;
    this->gps = cib.gps;

    this->httpServerInfo = cib.httpServerInfo;
    this->ainvrInfo = cib.ainvrInfo;
    this->appMonitorInfo = cib.appMonitorInfo;
    this->mediaKitInfo = cib.mediaKitInfo;
    this->serialThermalInfo = cib.serialThermalInfo;
    this->serialPTZInfo = cib.serialPTZInfo;
    this->str_mdls = cib.str_mdls;
    this->gb_ifo = cib.gb_ifo;
    return *this;
  }
  std::string httpServer;
  std::string env_heart;
  std::string cpu_heart;
  std::string cam_heart;
  std::string img_heart;
  std::string img_text_heart;
  std::string img_attach_heart;
  std::string net_heart;


  std::string runMode;
  std::string nodeName;
  std::string serialNumber;
  std::string nodeArch;
  std::string nodeId;
  std::string nodeIp;
  std::string configVersion;
  std::string modelDir;
  std::string mediaDir;
  std::string imgDir;
  std::string historyDir;
  int         historyDay;
  std::string gps;

  HttpServerInfo httpServerInfo;
  AiNvrInfo ainvrInfo;
  AppMonitorInfo appMonitorInfo;
  MediaKitInfo mediaKitInfo;
  SerialDataInfo serialThermalInfo;
  SerialDataInfo serialPTZInfo;
  std::vector<MonitorAppTask> tasks;
  std::vector<dev_con> Dev_con;
  std::vector<modbus_con> mdb_con;
  std::vector<camera> Camera_con;
  std::vector<Network> net;
  std::set<std::string> str_mdls;               // 模型字符串表示
  GB28181Info gb_ifo;
};


}   // namespace smartbox
