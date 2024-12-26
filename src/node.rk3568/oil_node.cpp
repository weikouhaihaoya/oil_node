#include "node.rk3568/oil_node.h"
//using namespace common;
using namespace cv;
namespace rk3568 {

    OilNode::OilNode(node::NodeConfigInfo* cfg_info, LogInfo* log_info):
    cfg_ifo(cfg_info), log_ifo(log_info) {

    }

    OilNode::~OilNode() {
        
    }
    

    void OilNode::send_node_heart_data() {
    int ret = 0;
    
    Json::Value root;
    root["eventType"] = "NodeHeart";
    //root["categoryId"] = cfg_ifo->categoryId;
    root["nodeName"] = cfg_ifo->nodeName;
    root["nodeId"] = cfg_ifo->serialNumber;
    root["nodeVersion"] = CUR_VERSION_RK;
    CheckDevice device = check_orangepi;
    root["nodeThermal"] = std::to_string(get_board_thermal(device));
    root["nodeThermal"] = "0";
    root["diskInfo"] = "0/0";
    root["nodeStatus"] = 0;
    
    DiskInfo du;
    ret = get_disk_percent(du);
    if (ret == 0) {root["diskInfo"] = std::to_string(du.used) +"/"+ std::to_string(du.total);}
    
    std::string msg = root.toStyledString();
    if (log_ifo->log_level_2) spdlog::get("logger")->info("AlgoNode::send_node_heart_data: \n{}", msg);
    
    // if (pub_cmd) { pub_cmd->publish_data(msg); } //将此信息转发一份出去给mqtt
    return ;
    }
std::string OilNode::getCurrentTime() {  
    // 获取当前时间点  
    auto now = std::chrono::system_clock::now();      

    // 将时间点转换为 time_t 类型，表示从1970年1月1日以来的秒数  
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);      

    // 将 time_t 类型转换为 tm 结构，表示本地时间  
    std::tm* now_tm = std::localtime(&now_time_t);       

    // 创建一个字符串流来构建时间字符串  
    std::ostringstream oss;  

    // 设置填充字符为0，宽度为2，用于格式化日期和时间  
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");  
    // 返回格式化后的时间字符串  
    return oss.str();  
}  
std::string OilNode::get_Data(std::string mod_ID){
    //ModbusAPI* mdb_api = new ModbusAPI();
    //Set_Modbus_con set_mod;
    //mdb_api->read_data_rtu();
    int ret=0;
    std::string msg;
    Json::Value root;
    mod_lock.lock();
    std::vector<uint16_t> DATA;
    int key=-1;
    for(int i=0;i<cfg_ifo->mdb_con.size();i++){
      if(cfg_ifo->mdb_con[i].mod_ID==mod_ID){
        if(cfg_ifo->mdb_con[i].modbusIdx<0||cfg_ifo->mdb_con[i].modbusIdx>7){
          //产生虚拟数据
          key=i;
          for(int j=0;j<cfg_ifo->mdb_con[key].readNumbers;j++){
            DATA.push_back(20);
          }
          goto end;
        }
      }

    }
    mod_con_lock.lock();
    ret=mdb->read_data_rtu(mod_ID);
    for(int i=0;i<mdb->data.size();i++){
      if(mdb->data[i].first==mod_ID){
        for(int j=0;j<mdb->data[i].second.size();j++){
          DATA.push_back((mdb->data[i].second)[j]);
        }
      }
    }
    mod_con_lock.unlock();

end:
    mod_lock.unlock();
    root["happenTime"]=getCurrentTime();
    root["mod_ID"]=mod_ID;
    root["data"]=Json::arrayValue;
    for(int i=0;i<DATA.size();i++){
      root["data"].append(DATA[i]);
    }
    msg=root.toStyledString();
    //delete mdb_api;
    return msg;
}
int OilNode::get_config(){
    modbus_con mod;
    mod_lock.lock();
    mod_con_lock.lock();
    for(int i=0;i<cfg_ifo->mdb_con.size();i++){
          //先扫描mdb中是否已经有对应的设备配置
          bool istrue=0;
          int key=-1;
          for(int j=0;j<mdb->mdb_config.size();j++){
            if(cfg_ifo->mdb_con[i].mod_ID==mdb->mdb_config[j].mod_ID){
              istrue=1;
              key=j;
              break;
            }
          }
          mod.modbusIdx=cfg_ifo->mdb_con[i].modbusIdx;
          mod.modebusType=cfg_ifo->mdb_con[i].modebusType;
          mod.mod_add=cfg_ifo->mdb_con[i].mod_add;
          mod.readNumbers=cfg_ifo->mdb_con[i].readNumbers;
          mod.Interval_time=cfg_ifo->mdb_con[i].Interval_time;
          mod.Baud_rate=cfg_ifo->mdb_con[i].Baud_rate;
          mod.Data_bit=cfg_ifo->mdb_con[i].Data_bit;
          mod.Parity=cfg_ifo->mdb_con[i].Parity;
          mod.Stop_bit=cfg_ifo->mdb_con[i].Stop_bit;
          mod.mod_ID=cfg_ifo->mdb_con[i].mod_ID;

          //串口配置
        ModbusAPI::modbus_config modd;
        modd.mod_ID=mod.mod_ID;
        std::cout<<"串口配置:"<<std::endl;
        std::cout<<modd.mod_ID<<std::endl;

        modd.mod_add=mod.mod_add;
        std::cout<<modd.mod_add<<std::endl;

        modd.readNumbers=mod.readNumbers;
        std::cout<<modd.readNumbers<<std::endl;

        switch(mod.modbusIdx){
            case 0:
                  modd.modbus_path=modbus_path_0;
                  break;
            case 1:
                  modd.modbus_path=modbus_path_1;
                  break;
            case 2:
                  modd.modbus_path=modbus_path_2;
                  break;
            case 3:
                  modd.modbus_path=modbus_path_3;
                  break;
            case 4:
                  modd.modbus_path=modbus_path_4;
                  break; 
            case 5:
                  modd.modbus_path=modbus_path_5;
                  break;
            case 6:
                  modd.modbus_path=modbus_path_6;
                  break;
            case 7:
                  modd.modbus_path=modbus_path_7;
                  break;
            default:
                  modd.modbus_path=modbus_path_0;
                  
         }
        std::cout<<modd.modbus_path<<std::endl;

        modd.Baud_rate=mod.Baud_rate;

        std::cout<<modd.Baud_rate<<std::endl;

        modd.Data_bit=mod.Data_bit;
        std::cout<<modd.Data_bit<<std::endl;

        modd.Parity=mod.Parity;
        //std::cout<<"mod.parity:"<<std::endl;
        std::cout<<modd.Parity<<std::endl;

        modd.Stop_bit=mod.Stop_bit;
        std::cout<<modd.Stop_bit<<std::endl;


        if(mod.modebusType=="RS485"){
              modd.modbus_mode=modbus_485;
        }
        else if(mod.modebusType=="RS232"){
              modd.modbus_mode=modbus_232;

        }
        std::cout<<modd.modbus_mode<<std::endl;

      //如果当前设备不存在则加入配置
      if(!istrue){
        mdb->mdb_config.push_back(modd);
      }
      else{
        //当前设备存在则更新配置
        mdb->mdb_config[key]=modd;
        //break;
      }

    }
    mod_con_lock.unlock();
    mod_lock.unlock();
    std::cout<<"mdb设备个数:"<<mdb->mdb_config.size()<<std::endl;


    // for(int i=0;i<cfg_ifo->Camera_con.size();i++){
      
    //   cam->ip=cfg_ifo->Camera_con[i].IP;
    //   std::cout<<cam->ip<<std::endl;

    //   cam->passwd=cfg_ifo->Camera_con[i].passwd;
    //   std::cout<<cam->passwd<<std::endl;

    //   cam->user=cfg_ifo->Camera_con[i].user;
    //   std::cout<<cam->user<<std::endl;

    //   cam->brand=cfg_ifo->Camera_con[i].brand;
    //   std::cout<<cam->brand<<std::endl;

    //   this->cam->Grab_time=cfg_ifo->Camera_con[i].Grab_time;
    //   std::cout<<cam->Grab_time<<std::endl;

    // }



}
std::string OilNode::getCpuInfo(){

    //std::string return_msg;
    int ret = 0;
    Json::Value root;
    //Json::Value json_data;
    //ret = parse_json(body, json_data, false);
    //std::string nodeId=json_data.get("nodeId",Json::Value("")).asString();
    // root["code"] = 0;
    // root["msg"] = "success";
    //root["data"]["categoryId"]="";
    root["nodeName"]=cfg_ifo->nodeName;
    root["nodeId"]=cfg_ifo->nodeId; 
    root["nodeVersion"]=CUR_VERSION_RK;
    root["diskInfo"]= "0/0";
    root["nodeIp"]=cfg_ifo->nodeIp;
    DiskInfo du;
    ret = get_disk_percent(du);
    if (ret == 0) {root["diskInfo"]= std::to_string(du.used) +"/"+ std::to_string(du.total);}
    
    CheckDevice device = check_orangepi;
    float dev_tem=get_board_thermal(device);
    dev_tem=round(dev_tem*100)/100;
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << dev_tem;

    root["nodeThermal"]=ss.str();
    root["nodeStatus"]=0;
    root["happenTime"]=getCurrentTime();

    CPU_OCCUPY cpu_stat1;    
    CPU_OCCUPY cpu_stat2;    
    MEM_OCCUPY mem_stat;   
    float cpu;         
        
    //第一次获取cpu使用情况   
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);   
    sleep(10);          
     //第二次获取cpu使用情况  
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);          
     //计算cpu使用率     
    cpu = cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2); 
    // std::cout<<round(cpu*100)/100<<std::endl;;
    // std::cout<<floor(cpu*100)/100<<std::endl;;
    // std::cout<<ceil(cpu*100)/100<<std::endl;;

     cpu=round(cpu*100)/100;
    // std::cout<<"cpu:"<<cpu<<std::endl;
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << cpu;
    root["cpu_occupy"]=oss.str();
    
    std::ostringstream osss;
    float mem=get_memoccupy ((MEM_OCCUPY *)&mem_stat);
    osss << std::fixed << std::setprecision(2) << mem;
    root["mem_occupy"]= osss.str();



    //root["version"] = CUR_VERSION_RK;
    return root.toStyledString();

}
// std::string OilNode::contrlAlarmDevice(){
//     std::string return_msg;
//     int ret = 0;
//     Json::Value root;
//     root["code"] = 0;
//     root["msg"] = "success";
//     root["data"]["status"]=true;
//     root["data"]["contrl_cmd"]=true;
//     //root["version"] = CUR_VERSION_RK;
//     return_msg = root.toStyledString();
//     return return_msg;
// }


int OilNode::setModbus(std::string body, std::string& ret_str) {
  //ModbusAPI* mdb_api = new ModbusAPI();
  
  int ret = 0;
  std::cout<<"body:"<<body<<std::endl;
  if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::setModbus. body:{}", body);

  Json::Value json_data;
  ret = parse_json(body, json_data, false);
  Json::Value root;
  std::cout<<"ret:"<<ret<<std::endl;
  if (ret != 0) { return ret; }


  //Set_mod set_mod;
  //modbus_idx=json_data["modbusIdx"].asInt();
  //std::cout<<modbus_idx<<std::endl;
  //string name=json_data["Module_name"].asString();
  modbus_con mod;
  //mod.modbusIdx= json_data["modbusIdx"].asInt();
  mod.modbusIdx=json_data.get("modbusIdx",Json::Value(-1)).asInt();
  // if(mod.modbusIdx<0||mod.modbusIdx>7){
  //   return MODBUSIDX_ERROR;
  // }
  std::cout<<mod.modbusIdx<<std::endl;

  //mod.modebusType=json_data["modebusType"].asString();
  mod.modebusType=json_data.get("modebusType",Json::Value("485")).asString();
  if(mod.modebusType!="RS232"&&mod.modebusType!="RS485"){
    return MODBUS_TYPE_ERROR;
  }
  std::cout<<mod.modebusType<<std::endl;


  //mod.mod_add=json_data["mod_add"].asInt();
  mod.mod_add=json_data.get("mod_add",Json::Value(0)).asInt();
  if(mod.mod_add<0){
    return MODBUS_ADD_ERROR;
  }
  std::cout<<mod.mod_add<<std::endl;

  //mod.readNumbers=json_data["readNumbers"].asInt();
  mod.readNumbers=json_data.get("readNumbers",Json::Value(1)).asInt();
  if(mod.readNumbers<=0){
    return MODBUS_READNUMBERS_ERROR;
  }
  std::cout<<mod.readNumbers<<std::endl;


  //mod.mod_ID=json_data["mod_ID"].asString();
  mod.mod_ID=json_data.get("mod_ID",Json::Value("")).asString();
  std::cout<<mod.mod_ID<<std::endl;

  //mod.Interval_time=json_data["Interval_time"].asInt()
  mod.Interval_time=json_data.get("Interval_time",Json::Value(1)).asInt();
  if(mod.Interval_time<=0){
    return MODBUS_Interval_time_ERROR;
  }
  std::cout<<mod.Interval_time<<std::endl;

  //mod.Baud_rate=json_data["Baud_rate"].asInt();
  mod.Baud_rate=json_data.get("Baud_rate",Json::Value(9600)).asInt();
  if(mod.Baud_rate<=0){
    return MODBUS_BOUD_RATE_ERROR;
  }
  std::cout<<mod.Baud_rate<<std::endl;

  //mod.Data_bit=json_data["Data_bit"].asInt();
  mod.Data_bit=json_data.get("Data_bit",Json::Value(8)).asInt();
  if(mod.Data_bit<=0){
    return MODBUS_DATA_BIT_ERROR;
  }
  std::cout<<mod.Data_bit<<std::endl;

  //mod.Parity=(json_data["Parity"].asString())[0];
  mod.Parity=(json_data.get("Parity",Json::Value("N")).asString())[0];
  if(mod.Parity!='N'&&mod.Parity!='O'){
    return MODBUS_PARITY_ERROR;
  }
  std::cout<<mod.Parity<<std::endl;

  //mod.Stop_bit=json_data["Stop_bit"].asInt();
  mod.Stop_bit=json_data.get("Stop_bit",Json::Value(1)).asInt();
  if(mod.Stop_bit<0){
    return MODBUS_STOP_BIT_ERROR;
  }
  std::cout<<mod.Stop_bit<<std::endl;

  //mod.mod_pos=json_data["mod_pos"].asInt();
  //检测配置中是否包含信息
  mod_lock.lock();
  bool istrue=0;
  for(int i=0;i<cfg_ifo->mdb_con.size();i++){
        if(cfg_ifo->mdb_con[i].mod_ID==mod.mod_ID){
          //覆盖原先的信息
          istrue=1;
          cfg_ifo->mdb_con[i].modbusIdx=mod.modbusIdx;
          cfg_ifo->mdb_con[i].modebusType=mod.modebusType;
          cfg_ifo->mdb_con[i].mod_add=mod.mod_add;
          cfg_ifo->mdb_con[i].readNumbers=mod.readNumbers;
          cfg_ifo->mdb_con[i].mod_ID=mod.mod_ID;
          cfg_ifo->mdb_con[i].Interval_time=mod.Interval_time;
          cfg_ifo->mdb_con[i].Baud_rate=mod.Baud_rate;
          cfg_ifo->mdb_con[i].Data_bit=mod.Data_bit;
          cfg_ifo->mdb_con[i].Parity=mod.Parity;
          cfg_ifo->mdb_con[i].Stop_bit=mod.Stop_bit;
        }

  }
  if(!istrue){
      cfg_ifo->mdb_con.push_back(mod);
  }
  mod_lock.unlock();







  //串口配置
  get_config();



  if(body.compare("")==0){
    root["code"]=1;
    root["msg"]="failed";
  }
  else{
      //接收到数据
    root["code"] = 0;
    root["msg"] = "success";
  }

  ret_str = root.toStyledString();
  if (log_ifo->log_level_3) spdlog::get("logger")->info("APPMonitor::peer_restart. ret_str:{}", ret_str);
  //delete mdb_api;
  return 0;
}
int OilNode::box_restart(std::string body, std::string& ret_str){
     if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::box_restart. body:{}", body);

    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    if (ret != 0) { return ret; }
    Json::Value root;
    BOX_reboot=json_data["box_reboot"].asInt();
    if(BOX_reboot<0){
      return DEVICE_REBOOT_ERROR;
    }
    if(BOX_reboot==1){
      std::string cmd="reboot";
      std::cout<<cmd<<std::endl;
      system(cmd.c_str());
      root["code"] = 0;
      root["msg"] = "success";
    }
    else{
        root["code"] = 1;
        root["msg"] = "falied";
    }
    ret_str = root.toStyledString();
    return 0;




}
int OilNode::dev_restart(std::string body, std::string& ret_str){
    if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::dev_restart. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    if (ret != 0) { return ret; }
    Json::Value root;
    dev dev_info;
    //dev_info.dev_id=json_data["deviceID"].asString();
    dev_info.dev_id=json_data.get("deviceID",Json::Value("")).asString();
    
    //dev_info.dev_name=json_data["dev_name"].asString();
    bool isfind=0;
    //int key=-1;
    dev_lock.lock();
    for(int i=0;i<cfg_ifo->Dev_con.size();i++){
      if(cfg_ifo->Dev_con[i].deviceID==dev_info.dev_id){
        dev_info.dev_gpio=cfg_ifo->Dev_con[i].gpio;
        dev_info.restart_time=cfg_ifo->Dev_con[i].restart_time;
        isfind=1;
        //key=i;
        break;
      }
    }
    dev_lock.unlock();
    if(!isfind){
      return DEVICE_ID_ERROR;
    }
    std::string cmd="";
    int gpio=-1;
    if(dev_info.dev_gpio<1||dev_info.dev_gpio>10){
      goto end;
    }
    //echo 13 > /sys/class/gpio/export
    //echo out > /sys/class/gpio/gpio13/direction
    //echo 1 > /sys/class/gpio/gpio13/value


    //std::cout<<cmd<<std::endl;
    //system(cmd.c_str());
    gpio=dev_map[dev_info.dev_gpio];
    // cmd="cd /userdata/service/mqtt/  && ./iotest -s "+to_string(gpio)+" 1";
    // std::cout<<cmd<<std::endl;
    // system(cmd.c_str());
    cmd="echo "+to_string(gpio)+" > /sys/class/gpio/export";
    std::cout<<cmd<<std::endl;
    system(cmd.c_str());

    cmd="echo out > /sys/class/gpio/gpio"+to_string(gpio)+"/direction";
    std::cout<<cmd<<std::endl;
    system(cmd.c_str());
    //std::this_thread::sleep_for(seconds(5));
    //先断电
    cmd="echo 1 > /sys/class/gpio/gpio"+to_string(gpio)+"/value";
    std::cout<<cmd<<std::endl;
    system(cmd.c_str());
    
    sleep(dev_info.restart_time);

    //在上电
    cmd="echo 0 > /sys/class/gpio/gpio"+to_string(gpio)+"/value";
    std::cout<<cmd<<std::endl;
    system(cmd.c_str());
    // cmd="cd /userdata/service/mqtt/  && ./iotest -s "+to_string(gpio)+" 0";
    // std::cout<<cmd<<std::endl;
    // system(cmd.c_str());
end:
    root["code"] = 0;
    root["msg"] = "success";
    ret_str = root.toStyledString();
    return 0;


}
int OilNode::ctlAlarmDevice(std::string body, std::string& ret_str){
  if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::ctlAlarmDevice. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    if (ret != 0) { return ret; }

    Json::Value root;
    dev dev_info;
    dev_info.dev_id=json_data["Alarmid"].asString();
    //dev_info.dev_name=json_data["dev_name"].asString();
    bool isfind=0;
    dev_lock.lock();
    for(int i=0;i<cfg_ifo->Dev_con.size();i++){
      if(cfg_ifo->Dev_con[i].deviceID==dev_info.dev_id){
        dev_info.dev_gpio=cfg_ifo->Dev_con[i].gpio;
        isfind=1;
        break;
      }
    }
    dev_lock.unlock();
    if(!isfind){
      return DEVICE_ID_ERROR;
    }
    int isquery=0;
    isquery=json_data["status"].asBool();
    int ctl_cmd=json_data["contrl_cmd"].asInt();
    int gpio=dev_map[dev_info.dev_gpio];
    std::string cmd="";
    cmd="echo "+to_string(gpio)+" > /sys/class/gpio/export";
    std::cout<<cmd<<std::endl;
    system(cmd.c_str());

    cmd="echo out > /sys/class/gpio/gpio"+to_string(gpio)+"/direction";
    std::cout<<cmd<<std::endl;
    system(cmd.c_str());


    if(ctl_cmd==1){
      //打开声光报警器
      cmd="echo 1 > /sys/class/gpio/gpio"+to_string(gpio)+"/value";
      std::cout<<cmd<<std::endl;
      system(cmd.c_str());
    }
    else{
        cmd="echo 0 > /sys/class/gpio/gpio"+to_string(gpio)+"/value";
        std::cout<<cmd<<std::endl;
        system(cmd.c_str());
    }
    std::string file;
    file="/sys/class/gpio/gpio"+to_string(gpio)+"/value";

    ifstream ifs(file);

    char c=ifs.get();
    //查询状态为设置之后的状态
    if(isquery){
      if(c=='1'){
        root["data"]["status"]=true;
        root["data"]["contrl_cmd"]=1;
      }
      else{
          root["data"]["status"]=false;
          root["data"]["contrl_cmd"]=1;
      }
    }
    root["code"]=0;
    root["msg"]="success";
    ret_str = root.toStyledString();
    return 0;




}


int OilNode::Grab_picture(std::string body, std::string& ret_str){
  if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::Grab_picture. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    if (ret != 0) { return ret; }
    Json::Value root;
    std::string return_msg;
    std::string camID=json_data.get("cameraID", Json::Value("11")).asString();
    cam_lock.lock();
    int key=-1;
    for(int i=0;i<cfg_ifo->Camera_con.size();i++){
      if(cfg_ifo->Camera_con[i].cameraID==camID){
        key=i;
        break;
      }
    }
    //配置文件中没有此相机
    if(key<0){
      cam_lock.unlock();
      return CAMERA_ID_NOT_EXIST;
    }
    
    string rtsp_url = "";
    std::string chl;
    if(cfg_ifo->Camera_con[key].brand=="Dahua"){
      std::string Dahua_fmt = "rtsp://{0}:{1}@{2}:554/cam/realmonitor?channel={3}&subtype=0";
      rtsp_url = util::Format(Dahua_fmt, cfg_ifo->Camera_con[key].user, cfg_ifo->Camera_con[key].passwd, cfg_ifo->Camera_con[key].IP, 1);

    }
    else if(cfg_ifo->Camera_con[key].brand=="Hikvision"){
        std::string Hikvision_fmt = "rtsp://{0}:{1}@{2}:554/ch{3}/main/av_stream";
        chl = (1 < 10 ? "0" : "") + std::to_string(1);
        rtsp_url = util::Format(Hikvision_fmt, cfg_ifo->Camera_con[key].user ,cfg_ifo->Camera_con[key].passwd, cfg_ifo->Camera_con[key].IP, chl);

    }
    cam_lock.unlock();
    std::cout<<"流地址："<<rtsp_url<<std::endl;
    if(rtsp_url==""){
      return get_stream_url_ERROR;
    }
    Mat frame;
    VideoCapture cap(rtsp_url);

    if (!cap.isOpened()) {
        cerr << "Error: Could not open video stream." << endl;
        return CAN_NOT_OPEN_VIDEO_STREAM;
    }
    cap >> frame;
    auto current_time = chrono::system_clock::now();
    std::string cur_time=getCurrentTime();
    std::string img_path = "";

    for(int i=0;i<cur_time.size();i++){
      if(cur_time[i]!='-'&&cur_time[i]!=' '&&cur_time[i]!=':'){
        img_path+=cur_time[i];
      }
    } 
    img_path+="_"+camID+".jpg";
    imwrite(img_path, frame);
    time_t now = chrono::system_clock::to_time_t(current_time);
    cout << "Captured image at: " << ctime(&now);



    //ret=get_img_by_rtsp(rtsp_url,frame);
    if(frame.empty()){
      return PICTURE_IS_EMPTY;
    }
    std::string file_url="";
         
    //发送图片
    ret = post_file(file_url, cfg_ifo->httpServer, cfg_ifo->img_heart, img_path, "url", 3, 3, log_ifo);
    std::cout<<"图片地址:"<<file_url<<std::endl;

    std::string msg="";
    root["data"]["happenTime"]=getCurrentTime();
    root["data"]["nodeId"]=cfg_ifo->nodeId;
    root["data"]["channelId"]="ch01";
    //std::string cur_time=getCurrentTime();
    std::string eventID="";
    for(int i=0;i<cur_time.size();i++){
        if(cur_time[i]!=' '&&cur_time[i]!='-'&&cur_time[i]!=':'){
          eventID+=cur_time[i];
        }
    }
    eventID+="_"+camID;
    root["data"]["eventId"]=eventID;

    //std::string ret_body;

    //发送图片的文本信息
    //post_data_base(ret_body, msg,  cfg_ifo->httpServer, cfg_ifo->img_text_heart, -1, 1, log_ifo);
    
    //发送图片的附件信息
    root["data"]["imgUrl"]=file_url;

    //post_data_base(ret_body, msg,  cfg_ifo->httpServer, cfg_ifo->img_attach_heart, -1, 1, log_ifo);

    root["code"]=0;
    root["msg"]="success";
    ret_str=root.toStyledString();
    std::cout<<ret_str<<std::endl;
    return 0;


    
}
std::string OilNode::Camera_Status(){
    cam_lock.lock();
    for(int i=0;i<cfg_ifo->Camera_con.size();i++){
      string rtsp_url = "";
      std::string chl;
      if(cfg_ifo->Camera_con[i].brand=="Dahua"){
        std::string Dahua_fmt = "rtsp://{0}:{1}@{2}:554/cam/realmonitor?channel={3}&subtype=0";
        rtsp_url = util::Format(Dahua_fmt, cfg_ifo->Camera_con[i].user, cfg_ifo->Camera_con[i].passwd, cfg_ifo->Camera_con[i].IP, 1);

      }
      else if(cfg_ifo->Camera_con[i].brand=="Hikvision"){
          std::string Hikvision_fmt = "rtsp://{0}:{1}@{2}:554/ch{3}/main/av_stream";
          chl = (1 < 10 ? "0" : "") + std::to_string(1);
          rtsp_url = util::Format(Hikvision_fmt, cfg_ifo->Camera_con[i].user ,cfg_ifo->Camera_con[i].passwd, cfg_ifo->Camera_con[i].IP, chl);

      }

      Mat frame;
      VideoCapture cap(rtsp_url);

      if (!cap.isOpened()) {
          cerr << "Error: Could not open video stream." << endl;
          //return CAN_NOT_OPEN_VIDEO_STREAM;
          //return "";
      }
      cap >> frame;
      //int ret=get_img_by_rtsp(rtsp_url,frame);

      if (frame.empty()) {
          std::cerr << "Error: Frame is empty" <<std::endl;
          status.insert({cfg_ifo->Camera_con[i].cameraID,0});
          
      }
      else{
          status.insert({cfg_ifo->Camera_con[i].cameraID,1});

      }

    }
    cam_lock.unlock();
    std::string msg;
    Json::Value root;
    root["Camera_status"]=Json::arrayValue;
    for(std::map<std::string,int>::iterator it=status.begin();it!=status.end();it++){
      Json::Value r;
      r["cameraID"]=it->first;
      r["status"]=it->second;
      root["Camera_status"].append(r);
    }
    msg=root.toStyledString();
    return msg;
}
int OilNode::set_Camera(std::string body, std::string& ret_str){
  if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::set_Camera. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false); 
    if (ret != 0) { return ret; }
    
    Json::Value root;
    camera came;
    
    came.IP=json_data["IP"].asString();
    came.passwd=json_data["passwd"].asString();
    came.user=json_data["user"].asString();
    came.brand=json_data["brand"].asString();
    //came.Grab_time=json_data["Grab_time"].asInt();
    came.cameraID=json_data["cameraID"].asString();
    cam_lock.lock();
    bool istrue=0;
    //如果写入一个重复的则进行覆盖
    for(int i=0;i<cfg_ifo->Camera_con.size();i++){
      if(cfg_ifo->Camera_con[i].IP==came.IP){
        cfg_ifo->Camera_con[i].IP=came.IP;
        cfg_ifo->Camera_con[i].passwd=came.passwd;
        cfg_ifo->Camera_con[i].user=came.user;
        cfg_ifo->Camera_con[i].brand=came.brand;
        cfg_ifo->Camera_con[i].cameraID=came.cameraID;
        //cfg_ifo->Camera_con[i].Grab_time=came.Grab_time;
        istrue=1;
      }
    }
    if(!istrue){
      cfg_ifo->Camera_con.push_back(came);
    }
    cam_lock.unlock();




    //cam.push_back(cam_con);
    root["code"] = 0;
    root["msg"] = "success";

    ret_str = root.toStyledString();

    return 0;

}
int OilNode::set_relay(std::string body, std::string& ret_str){
  if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::set_relay. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    if (ret != 0) { return ret; }
    Json::Value root;
    dev_con dev;  

    dev.deviceID=json_data.get("deviceID",Json::Value("")).asString();
    
    dev.gpio=json_data.get("gpio",Json::Value(0)).asInt();

    
    dev.restart_time=json_data.get("restart_time",Json::Value(30)).asInt();

    // if(dev.gpio<1||dev.gpio>6){
    //   //输入的gpio错误
    //   return DEVICE_GPIO_ERROR;
    // }
    //判断是否已经包含该设备
    dev_lock.lock();
    bool isexist=0;
    for(int i=0;i<cfg_ifo->Dev_con.size();i++){
      if(cfg_ifo->Dev_con[i].deviceID==dev.deviceID){
        //包含该设备
        //进行配置更新
        cfg_ifo->Dev_con[i].gpio=dev.gpio;
        cfg_ifo->Dev_con[i].restart_time=dev.restart_time;
        isexist=1;
        break;
      }
    }
    if(!isexist){
      cfg_ifo->Dev_con.push_back(dev);
    }
    dev_lock.unlock();
    root["code"]=0;
    root["msg"]="success";
    ret_str = root.toStyledString();

    return 0;
}


int OilNode::delete_device(std::string body, std::string& ret_str){
  if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::delete_device. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    if (ret != 0) { return ret; }
    Json::Value root;
    if(json_data.isMember("cameraID")){
      //std::cout<<"111"<<std::endl;
      //要删除的设备是相机
      //删除相机配置
      bool isfind_cam=0;
      cam_lock.lock();
      std::string cam_ID=json_data.get("cameraID",Json::Value("")).asString();
      if(cfg_ifo->Camera_con.size()==0){
        cam_lock.unlock();
        return CAM_EMPTY_ERROR;
      }

      for(std::vector<camera>::iterator it=cfg_ifo->Camera_con.begin();it!=cfg_ifo->Camera_con.end();){
        if(it->cameraID==cam_ID){
          isfind_cam=1;
          it=cfg_ifo->Camera_con.erase(it);
          //std::cout<<"111"<<std::endl;
        }
        else{
          it++;
        }
      }
      for(auto it=status.begin();it!=status.end();){
        if(it->first==cam_ID){
          it=status.erase(it);
        }
        else{
          it++;
        }
      }
      //std::cout<<"222"<<std::endl;
      cam_lock.unlock();

      if(!isfind_cam){
        root["code"]=1;
        root["msg"]="failed";
        ret_str = root.toStyledString();
        return CAM_ID_ERROR;
      }


      root["code"]=0;
      root["msg"]="success";
      ret_str = root.toStyledString();
      
      return 0;

    }
    else if(json_data.isMember("deviceID")){
      //删除的设备是普通设备
      std::string device_ID=json_data.get("deviceID",Json::Value("")).asString();

      dev_lock.lock();
      bool isfind_dev=0;
      //删除继电器配置
      if(cfg_ifo->Dev_con.size()==0){
        dev_lock.unlock();
        return DEVICE_EMPTY_ERROR; 
      }
      for(std::vector<dev_con>::iterator it=cfg_ifo->Dev_con.begin();it!=cfg_ifo->Dev_con.end();){
          if(it->deviceID==device_ID){
            isfind_dev=1;
            it=cfg_ifo->Dev_con.erase(it);
          }
          else{
            it++;
          }
      }
      dev_lock.unlock();
      if(!isfind_dev){
        root["code"]=1;
        root["msg"]="failed";
        ret_str = root.toStyledString();
        return DEVICE_ID_ERROR;
      }

      root["code"]=0;
      root["msg"]="success";
      ret_str = root.toStyledString();
      return 0;
    }
    else if(json_data.isMember("mod_ID")){
      //删除modbus设备
      std::string mod_id=json_data.get("mod_ID",Json::Value("")).asString();
      //std::cout<<"mod_id:"<<mod_id<<std::endl;
      mod_lock.lock();
      if(cfg_ifo->mdb_con.size()==0){
        mod_lock.unlock();
        return MDB_EMPTY_ERROR;
      }

      bool isfind=0;
      for(std::vector<modbus_con>::iterator it=cfg_ifo->mdb_con.begin();it!=cfg_ifo->mdb_con.end();){
        if(it->mod_ID==mod_id){
          //std::cout<<"111"<<std::endl;
          //删除设备
          it=cfg_ifo->mdb_con.erase(it);
          //std::cout<<"222"<<std::endl;
          //找到设备
          isfind=1;
          
        }
        else{
          it++;
        }
      }
      mod_lock.unlock();
      mod_con_lock.lock();
      for(auto it=mdb->mdb_config.begin();it!=mdb->mdb_config.end();){
        if(it->mod_ID==mod_id){
          it=mdb->mdb_config.erase(it);
        }
        else{
          it++;
        }
      }
      mod_con_lock.unlock();
      //std::cout<<"333"<<std::endl;

      if(!isfind){
        //没有找到设备
        root["code"]=1;
        root["msg"]="failed";
        ret_str = root.toStyledString();
        return MOD_ID_ERROR;
      }
      root["code"]=0;
      root["msg"]="success";
      ret_str = root.toStyledString();
      //std::cout<<"444"<<std::endl;
      return 0;

    }
    return 0;


}
std::string OilNode::Network_detection(std::string ip){

  Json::Value root;

  float delay_total=monitor_device_online(ip);
  root["IP"]=ip;
  delay_total=round(delay_total*1000)/1000;
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(3) << delay_total;


  root["delay_total"]=oss.str();
  std::string msg=root.toStyledString();

  return msg;    
}
 

int OilNode::Network_configuration(std::string body, std::string& ret_str){
  if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::Network_configuration. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    if (ret != 0) { return ret; }
    Json::Value root;
    Network nett;
    nett.IP=json_data.get("IP",Json::Value("")).asString();
    nett.Interval_time=json_data.get("Interval_time",Json::Value(30)).asInt();
    net_lock.lock();
    bool isexist=0;
    for(int i=0;i<cfg_ifo->net.size();i++){
      if(cfg_ifo->net[i].IP==nett.IP){
        cfg_ifo->net[i].Interval_time=nett.Interval_time;
        isexist=1;
      }
    }
    if(!isexist){
      cfg_ifo->net.push_back(nett);
    }
    net_lock.unlock();
    root["code"]=0;
    root["msg"]="success";
    ret_str = root.toStyledString();

    return 0;

}

int OilNode::delete_Network(std::string body, std::string& ret_str){
  //写日志
    if (log_ifo->log_level_3) spdlog::get("logger")->info("OilNode::delete_Network. body:{}", body);
    Json::Value json_data;
    int ret = parse_json(body, json_data, false);
    //判断是否解析正确
    if (ret != 0) { return ret; }
    Json::Value root;
    std::string ip=json_data.get("IP",Json::Value("")).asString();
    net_lock.lock();
    bool isfind=0;
    if(cfg_ifo->net.size()==0){
      net_lock.unlock();
      return NET_EMPTY_ERROR;
    }
   for(auto it=cfg_ifo->net.begin();it!=cfg_ifo->net.end();){
        if(it->IP==ip){
          //删除设备
          it=cfg_ifo->net.erase(it);
          //找到设备
          isfind=1;
        }
        else{
          it++;
        }

    }
    net_lock.unlock();
    if(!isfind){
      //没有找到设备
      root["code"]=1;
      root["msg"]="failed";
      ret_str = root.toStyledString();
      return NET_IP_ERROR;
    }
    root["code"]=0;
    root["msg"]="success";
    ret_str = root.toStyledString();



    return 0;



}

float OilNode::get_memoccupy (MEM_OCCUPY *mem){
    FILE *fd;

    int n;

    char buff[256];

    char buff1[256];

    char buff2[256];

    char buff3[256];

    char buff4[256];

    MEM_OCCUPY *m;

    m=mem;

    float mem_use;

    fd = fopen ("/proc/meminfo", "r");

    fgets (buff1, sizeof(buff1), fd);

    //printf("%s\n",buff1);

    int num1 = atoi(buff1+15);

    //printf("%d\n",num1);

    fgets (buff2, sizeof(buff2), fd);

    //printf("%s\n",buff2);

    int num2 = atoi(buff2+15);

    fgets (buff3, sizeof(buff3), fd);

    //printf("%s\n",buff3);

    int num3 = atoi(buff3+15);

    fgets (buff4, sizeof(buff4), fd);

    // sscanf (buff, "%s %u %s", m->name, &m->total, m->name2);

    //printf("%s\n",buff4);

    int num4 = atoi(buff4+15);

    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里

    //printf("%s\n",buff);
    int num5 = atoi(buff+15);
    // 内存使用率(MEMUsedPerc)=100*(MemTotal-MemFree-Buffers-Cached)/MemTotal

    mem_use = (float)100*(num1-num2-num5-num4)/num1;

    //printf("内存使用率为：%.2f%%\n",mem_use);

    fclose(fd);     //关闭文件fd
    return ceil(mem_use*100)/100;
}

float OilNode::cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n){
    unsigned long od, nd;       
    unsigned long id, sd;     
    float cpu_use = 0;           
    od = (unsigned long) (o->user + o->nice + o->system +o->idle);//第一次(用户+优先级+系统+空闲)的时间再赋给od    
    nd = (unsigned long) (n->user + n->nice + n->system +n->idle);//第二次(用户+优先级+系统+空闲)的时间再赋给nd      
    id = (unsigned long) (n->user - o->user);    //用户第一次和第二次的时间之差再赋给id    
    sd = (unsigned long) (n->system - o->system);//系统第一次和第二次的时间之差再赋给sd  
    if((nd-od) != 0)
    {
         cpu_use = (float)((sd+id)*100)/(nd-od); //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used   
    }     
    else
    {   cpu_use = 0;          
       
    }   
    //printf("cpu使用率为:%1.2f%%\n",cpu_use);     
    return cpu_use; 
}

void OilNode::get_cpuoccupy (CPU_OCCUPY *cpust){
  FILE *fd;            
  int n;              
  char buff[256];    
  CPU_OCCUPY *cpu_occupy;     
  cpu_occupy=cpust;           
  fd = fopen ("/proc/stat", "r");    
  fgets (buff, sizeof(buff), fd);          
  sscanf (buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle);   
  fclose(fd);   
}
int OilNode::run() {
  int ret = 0;

  int64_t heart_time=0,post_time=0, cur_time=0, remove_time=0, task_time=0;
  task_time = remove_time = heart_time = post_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  //for (auto& task : cfg_ifo->tasks) {task.last_time = task_time;}
  //std::this_thread::sleep_for(seconds(60));
  int64_t cpu_time=0,env_time=0,camera_time=0;
  std::thread(remove_history_img, log_ifo->log_root, post_time, cfg_ifo->historyDay).detach();
  int logger_flag = 0;
  while (true) {

    cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    int hour = ((cur_time / 1000) % 86400) / 3600 + 8;
    hour = hour >= 24 ? hour - 24 : hour;

    std::string ret_body, env_msg,cpu_msg,msg,cam_msg,net_msg;

    //定时发送环境数据
    if(mdb->mdb_config.size()!=0){
      for(int i=0;i<cfg_ifo->mdb_con.size();i++){
        if (cur_time - cfg_ifo->mdb_con[i].env_time > cfg_ifo->mdb_con[i].Interval_time*1e3){
          cfg_ifo->mdb_con[i].env_time=cur_time;
          env_msg=get_Data(cfg_ifo->mdb_con[i].mod_ID);
          std::cout<<env_msg<<std::endl;
          post_data_base(ret_body, env_msg,  cfg_ifo->httpServer, cfg_ifo->env_heart, -1, 1, log_ifo);
          std::cout<<"环境数据返回信息:"<<ret_body<<std::endl;
        }
      }
    }


    //发送cpu信息
    if(cur_time-cpu_time>60*1e3){
      cpu_time=cur_time;
      cpu_msg=getCpuInfo();
      std::cout<<cpu_msg<<std::endl;
      post_data_base(ret_body, cpu_msg,  cfg_ifo->httpServer, cfg_ifo->cpu_heart, -1, 1, log_ifo);
    }

    //发送相机的状态信息
    if(cfg_ifo->Camera_con.size()!=0){
      if(cur_time-camera_time>60*1e3){
          camera_time=cur_time;
          cam_msg=Camera_Status();
          std::cout<<cam_msg<<std::endl;
          post_data_base(ret_body, cam_msg,  cfg_ifo->httpServer, cfg_ifo->cam_heart, -1, 1, log_ifo);

      }
    }

    //发送网络状态信息
    if(cfg_ifo->net.size()!=0){
      for(int i=0;i<cfg_ifo->net.size();i++){
        if(cur_time-cfg_ifo->net[i].net_time>cfg_ifo->net[i].Interval_time*1e3){
          cfg_ifo->net[i].net_time=cur_time;
          net_msg=Network_detection(cfg_ifo->net[i].IP);
          std::cout<<net_msg<<std::endl;
          post_data_base(ret_body, net_msg,  cfg_ifo->httpServer, cfg_ifo->net_heart, -1, 1, log_ifo);

        }
      }
    }

    //定时清除图片和日志
    if(cur_time-remove_time>10*60*1e3){
      //保留最近15天和从最一开始的14个日志文件
      remove_history_img(log_ifo->log_root, post_time, cfg_ifo->historyDay);
      int save_seconds=30*24*60*60;
      remove_file(cfg_ifo->imgDir, save_seconds, ".jpg");
      remove_time=cur_time;
    }


    //std::cout<<cpu_msg<<std::endl;
    // if(cur_time-env_time>cfg_ifo->mdb_con[0].Interval_time*1e3){
    //   env_time=cur_time;
    //   env_msg=get_Data(cfg_ifo->mdb_con[0].mod_ID);
    //   post_data_base(ret_body, env_msg,  cfg_ifo->httpServer, cfg_ifo->heart, -1, 1, log_ifo);

    // }
    // env_msg=get_Data(cfg_ifo->mdb_con[0].mod_ID);
    // post_data_base(ret_body, env_msg,  cfg_ifo->httpServer, cfg_ifo->heart, -1, 1, log_ifo);
    // 刷新每天的日志路径
    if (!logger_flag && hour == 1) {
      logger_flag = 1;
      spdlog::drop("logger");
      auto logger = spdlog::basic_logger_mt("logger", boost::filesystem::path(log_ifo->get_log_path()).append("log_" + get_today_date(false) + ".txt").string());
    }
    if (hour == 2 && logger_flag) { logger_flag = 0;}
  }
  return ret;
}

    // int OilNode::check_gpio_status(int gpio){
    //      std::string command="echo "+to   



    // }
    // void OilNode::get_CpuInfo(){



    // }

}