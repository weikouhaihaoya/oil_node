#include "video_tool.h"

using namespace std::chrono; 

namespace common {

int get_img_by_rtsp(std::string rtsp_url, cv::Mat& img) {
  int ret = 0;
  cv::VideoCapture reader = cv::VideoCapture(rtsp_url);;
  // if (!reader.open(rtsp_url)){ return VIDEO_OPEN_ERROR; }
  if (!reader.isOpened()) { return VIDEO_OPEN_ERROR; }
  
  bool ok = reader.read(img);
  if (!ok || img.empty()) {return VIDEO_READ_IMG_ERROR;}
  return 0;
}

int check_rtsp_is_ok(std::string rtsp_url) {
  int ret = 0;
  cv::Mat img;
  ret = get_img_by_rtsp(rtsp_url, img);
  if (ret != 0) {return ret;}
  return 0;
}

int save_video_by_nvr(LogInfo *log_ifo, algo::PostInfo& pi, NVRInfo nvr_ifo, std::string& writer_path) {

  // rtsp://admin:abcd1234@192.10.14.126:554/Streaming/tracks/201?starttime=20221007t150000z&endtime=20221007t150030z
  std::string starttime = time_int64_to_string(pi.start_time, "%Y%m%dt%H%M%Sz");
  std::string endtime = time_int64_to_string(pi.end_time, "%Y%m%dt%H%M%Sz");
  std::string rtsp = "rtsp://"+nvr_ifo.user+":"+nvr_ifo.passwd+"@"+nvr_ifo.NVRIp+":554/Streaming/tracks/"+nvr_ifo.NVRChannel+"01?starttime="+starttime+"&endtime="+endtime;
  if (log_ifo->log_level_1) spdlog::get("logger")->info("SmartBox::post_data_video_v2 rtsp: \n{}", rtsp);

  
  cv::VideoCapture reader = cv::VideoCapture(rtsp);;
  if (!reader.isOpened()) { 
    spdlog::get("logger")->info("ERROR. SmartBox::post_data_video_v2 !reader->isOpened() try_cnt: {}, rtsp: \n{}", pi.try_cnt, rtsp);
    if (pi.try_cnt < 10) {
      pi.try_cnt++;
    }
    return -1;
  }
  int width = reader.get(cv::CAP_PROP_FRAME_WIDTH);
  int height = reader.get(cv::CAP_PROP_FRAME_HEIGHT);
  double fps = reader.get(cv::CAP_PROP_FPS);
  if (fps < 1 || fps > 50) { fps = 15.;}
  if (log_ifo->log_level_1) spdlog::get("logger")->info("SmartBox::post_data_video_v2 width: {}, height: {}, fps: {}", width, height, fps);


  writer_path = bf::path(log_ifo->get_log_path()).append(std::to_string(pi.time) + ".mp4").string();
  cv::VideoWriter writer = cv::VideoWriter(writer_path, cv::VideoWriter::fourcc('a','v','c','1'), fps, cv::Size(width, height), true);
  int frame_cnt = 0;
  while(true) {
    cv::Mat cur_img;
    bool ok = reader.read(cur_img);
    if (!ok || cur_img.empty()) { break; }

    writer.write(cur_img); 
    frame_cnt++;
    if (log_ifo->log_level_1) spdlog::get("logger")->info("SmartBox::post_data_video_v2 writer frame_cnt: {}", frame_cnt);
  }
  reader.release();
  writer.release();

  return 0;
}

int save_video_by_kit(LogInfo *log_ifo, algo::PostInfo& post_ifo, std::string mediaDir, std::string& writer_path, std::string type) {
  std::vector<bf::path> total_videos;
  std::string sub_str = "www/record/" + type + "/" + post_ifo.task_ifo.channelId;
  std::string video_dir = bf::path(mediaDir).append(sub_str).string();
  get_files_in_dir(video_dir, total_videos, ".mp4");
  if (log_ifo->log_level_1) spdlog::get("logger")->info("total_videos: {}", total_videos.size());

  std::vector<std::pair<int64, std::string>> video_paths;
  for (auto& p : total_videos) {
    std::string filename = p.filename().string(); 
    if (filename.length() < 2) { 
      spdlog::get("logger")->info("ERROR. save_video_by_kit: {}", p.string());
      continue; 
    }
    if (p.filename().string().substr(0,1) == ".") { continue; }
    std::string time_str = p.parent_path().filename().string() + " " + p.filename().string().substr(0, 8);
    int64 time_stamp = 1000 * (int64)convert_time_str2time_stamp(time_str, "%Y-%m-%d %H-%M-%S");
    if (log_ifo->log_level_1) spdlog::get("logger")->info("time_str: {} time_stamp:{} ", time_str, time_stamp);
    video_paths.push_back(std::make_pair(time_stamp, p.string()));
  }
  std::sort(video_paths.begin(), video_paths.end(), [](std::pair<int64, std::string> &a, std::pair<int64, std::string> &b){ 
    return a.first < b.first;
  });
  if (log_ifo->log_level_1) {
    for (auto& p : video_paths) { spdlog::get("logger")->info("video_path: {} {} ", p.first, p.second);}
  }

  int video_idx = -1;
  for (size_t i = 0; i < video_paths.size(); i++) { 
    if (post_ifo.start_time >= video_paths[i].first) {video_idx = i;}
  }
  if (log_ifo->log_level_1) spdlog::get("logger")->info("video_idx: {}", video_idx);
  if (video_idx < 0) { return -1;}
  if (log_ifo->log_level_1) spdlog::get("logger")->info("video_idx: {}", video_paths[video_idx].first);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("video_idx: {}", video_paths[video_idx].second);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("post_ifo.start_time: {}, first: {}", post_ifo.start_time, video_paths[video_idx].first);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("post_ifo.end_time: {}, first: {}", post_ifo.end_time, video_paths[video_idx].first);

  cv::VideoCapture reader = cv::VideoCapture(video_paths[video_idx].second);
  if (!reader.isOpened()) {
    spdlog::get("logger")->info("ERROR. VideoCapture openc error. {}", video_paths[video_idx].second);
    return -1;
  }
  int width = reader.get(cv::CAP_PROP_FRAME_WIDTH);
  int height = reader.get(cv::CAP_PROP_FRAME_HEIGHT);
  double fps = reader.get(cv::CAP_PROP_FPS);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("width: {} height: {} fps: {}", width, height, fps);
  if (fps < 1 || fps > 50) { 
    fps = 15;
    spdlog::get("logger")->info("change fps: {}", fps);
  }

  int start_cnt = (post_ifo.start_time - video_paths[video_idx].first) / (1000 / fps);
  int end_cnt = (post_ifo.end_time - video_paths[video_idx].first) / (1000 / fps);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("start_cnt: {}", start_cnt);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("end_cnt: {}", end_cnt);
  
  writer_path = bf::path(log_ifo->get_log_path(post_ifo.task_ifo.channelId)).append(std::to_string(post_ifo.time) + "_"+post_ifo.algo_type+".mp4").string();
  cv::VideoWriter writer = cv::VideoWriter(writer_path, cv::VideoWriter::fourcc('a','v','c','1'), fps, cv::Size(width, height), true);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("writer_path: {}", writer_path);

  int error_cnt = 0;
  int frame_cnt = 0;
  while(frame_cnt < end_cnt) {
    cv::Mat cur_img;
    bool ok = reader.read(cur_img);

    if (!ok) {
      error_cnt++;
      // 提高容错性 部分视频前面n帧会读取失败 但frame_cnt不会变
      if (error_cnt > 500) {
        reader.release();
        if (video_idx+1 < video_paths.size()) {
          if (log_ifo->log_level_1) spdlog::get("logger")->info("video_idx: {}, video_paths: {}", video_idx+1, video_paths[video_idx+1].second);
          reader = cv::VideoCapture(video_paths[video_idx+1].second);
          error_cnt = 0;
        } else { break; }
      }
      continue;
    }

    if (frame_cnt >= start_cnt) { 
      if (log_ifo->log_level_1) spdlog::get("logger")->info("ok: {} frame_cnt: {}", ok, frame_cnt);
      writer.write(cur_img); 
    }
    frame_cnt++;
  }
  reader.release();
  writer.release();

  return 0;
}

int save_video_by_fmpg(LogInfo *log_ifo, algo::PostInfo& post_ifo, std::string mediaDir, std::string& writer_path, std::string type) {
  int ret = 0;
  std::vector<bf::path> total_videos;
  std::string sub_str = "www/record/" + type + "/" + post_ifo.task_ifo.channelId;
  std::string video_dir = bf::path(mediaDir).append(sub_str).string();
  get_files_in_dir(video_dir, total_videos, ".mp4");
  if (log_ifo->log_level_1) spdlog::get("logger")->info("total_videos: {}", total_videos.size());

  std::vector<std::pair<int64, std::string>> video_paths;
  for (auto& p : total_videos) {
    std::string filename = p.filename().string(); 
    if (filename.length() < 2) { 
      spdlog::get("logger")->info("ERROR. save_video_by_fmpg: {}", p.string());
      continue; 
    }
    if (p.filename().string().substr(0,1) == ".") { 
      if (log_ifo->log_level_1) spdlog::get("logger")->info("p:{} ", p.string());
      continue; 
    }
    std::string time_str = p.parent_path().filename().string() + " " + p.filename().string().substr(0, 8);
    int64 time_stamp = 1000 * (int64)convert_time_str2time_stamp(time_str, "%Y-%m-%d %H-%M-%S");
    if (log_ifo->log_level_1) spdlog::get("logger")->info("time_str: {} time_stamp:{} ", time_str, time_stamp);
    video_paths.push_back(std::make_pair(time_stamp, p.string()));
  }
  std::sort(video_paths.begin(), video_paths.end(), [](std::pair<int64, std::string> &a, std::pair<int64, std::string> &b){ 
    return a.first < b.first;
  });
  if (log_ifo->log_level_1) {
    for (auto& p : video_paths) { spdlog::get("logger")->info("video_path: {} {} ", p.first, p.second);}
  }

  int video_idx = -1;
  for (size_t i = 0; i < video_paths.size(); i++) { 
    if (post_ifo.start_time >= video_paths[i].first) {video_idx = i;}
  }
  if (log_ifo->log_level_1) spdlog::get("logger")->info("video_idx: {}", video_idx);
  if (video_idx < 0) { return -1;}
  cv::VideoCapture reader = cv::VideoCapture(video_paths[video_idx].second);
  if (!reader.isOpened()) {
    spdlog::get("logger")->info("ERROR. VideoCapture openc error. {}", video_paths[video_idx].second);
    return -1;
  }
  double frameRate_o = reader.get(cv::CAP_PROP_FPS);
  int frameCount = reader.get(cv::CAP_PROP_FRAME_COUNT);
  int total_len_time = frameCount / frameRate_o;
  reader.release();
  int64 next_post_time = video_paths[video_idx].first + total_len_time * 1000;
  if (log_ifo->log_level_1) spdlog::get("logger")->info("next_post_time: {},post_ifo.start_time:{}", next_post_time,post_ifo.start_time);
  if(post_ifo.start_time >= next_post_time ){
    return -1;
  }
  if (log_ifo->log_level_1) spdlog::get("logger")->info("video_idx: {}", video_paths[video_idx].first);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("video_idx: {}", video_paths[video_idx].second);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("post_ifo.start_time: {}, first: {}", post_ifo.start_time, video_paths[video_idx].first);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("post_ifo.end_time: {}, first: {}", post_ifo.end_time, video_paths[video_idx].first);

  writer_path = bf::path(log_ifo->get_log_path(post_ifo.task_ifo.channelId)).append(std::to_string(post_ifo.time) + "_"+post_ifo.algo_type+".mp4").string();
  
  // Saver* saver = new Saver(log_ifo);
  Saver* saver = new Saver();

  ret = saver->open_mp4(post_ifo,video_paths[video_idx].second,video_paths[video_idx].first,total_len_time,false);
  if(ret != 0){spdlog::get("logger")->info("ERROR. open_mp4 openc error. {}",ret);delete saver;return ret;}
  //第一次查看是否是有2个视频。查看完关闭，从新打开
  avformat_close_input(&saver->inputFormatContext);
  //left_durationInSeconds 说明跨越了2个视频
  int for_cnt = saver->left_durationInSeconds == 0 ? 1 : 2;
  std::string temp_dir = "temp";
  if(!bf::exists(temp_dir)) {bf::create_directory(temp_dir);}
  for(int index = 0;index < for_cnt;index++){
    bool is_seek = false;
    if(index == 0){is_seek = true;}
    ret = saver->open_mp4(post_ifo,video_paths[video_idx + index].second,video_paths[video_idx + index].first,total_len_time,is_seek);
    if(ret != 0){spdlog::get("logger")->info("ERROR. open_mp4 openc error. {}",ret);delete saver;return ret;}

    std::string out_path = writer_path; 
    std::string sub_str = std::to_string(index) + ".mp4";
    std::string temp_path = bf::path(temp_dir).append(sub_str).string();
    if(for_cnt == 2){out_path = temp_path;}

    ret = saver->set_out_mp4(out_path);
    if(ret != 0){spdlog::get("logger")->info("ERROR. set_out_mp4 error. {}",ret);delete saver;return ret;}

    // 从起始时间开始复制数据
    int error_cnt = 0;
    int64_t totalTime = 0;
    AVPacket packet;
    int frame_cnt = 0;
    int64_t pts_t = 0;
    int first_pts = 1;
    int64_t fir_pts = 0;
    int64_t fir_dts = 0;
    while (true){
      int ok = av_read_frame(saver->inputFormatContext, &packet);
      if (ok < 0) {
        error_cnt++;
        // 提高容错性 部分视频前面n帧会读取失败 但frame_cnt不会变
        if (error_cnt > 500) {break;}
        continue;
      }
      AVRational time_base = saver->inputFormatContext->streams[saver->videoStreamIndex]->time_base;
      AVRational outputTimeBase = saver->outputFormatContext->streams[saver->videoStreamIndex]->time_base;
      int64_t pts = av_rescale_q(packet.pts, time_base, AV_TIME_BASE_Q);
      if (pts <= saver->endTimestamp) {
        // 计算当前帧的时间戳
        int64_t duration = av_rescale_q_rnd(packet.duration, time_base, outputTimeBase,AVRounding(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        int64_t pts_t = av_rescale_q_rnd(packet.pts, time_base, outputTimeBase,AVRounding(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        int64_t dts_t = av_rescale_q_rnd(packet.dts, time_base, outputTimeBase,AV_ROUND_NEAR_INF);
        if(first_pts){
          fir_pts = pts_t;
          fir_dts = dts_t;
          first_pts = 0;
        }
        // 调整时间戳
        packet.pts = pts_t - fir_pts;
        packet.dts = dts_t - fir_dts;
        packet.duration = duration;
        packet.pos = -1;
        frame_cnt++;
        // 写入数据
        av_interleaved_write_frame(saver->outputFormatContext, &packet);

      }else{av_packet_unref(&packet);break;}

      // 释放数据包
      av_packet_unref(&packet);
    }
    std::cout<<"frame_cnt:"<<frame_cnt<<std::endl;

    av_write_trailer(saver->outputFormatContext);
    if (saver->outputFormatContext && !(saver->outputFormatContext->flags & AVFMT_NOFILE)) {
      avio_closep(&saver->outputFormatContext->pb);//avio打开要释放
    }
    avformat_close_input(&saver->inputFormatContext);
    avformat_free_context(saver->outputFormatContext);
  }

  //如果有两个文件，将两个文件进行合并为一个视频。
  if(saver->left_durationInSeconds != 0){
    std::string file_one = bf::path(temp_dir).append("0.mp4").string();
    std::string file_two = bf::path(temp_dir).append("1.mp4").string();
    //将第二个视频添加到第一个视频里面
    ret = saver->contact_two_mp4(file_one,file_two,writer_path);
    if(ret<0){
      std::cout<<"contact_two_mp4 error ret :"<<ret<<std::endl;
      spdlog::get("logger")->info("ERROR. contact_two_mp4 error. {}",ret);
      delete saver;
      return -1;
    }

    //合并完成删除temp下对应的文件。
    remove_file(temp_dir,0,".mp4");  //测试查看
  }

  saver->left_durationInSeconds = 0; //将其恢复为0
  saver->seekTarget = 0;
  saver->endTimestamp = 0;
  delete saver;
  return 0;
}

int save_video_by_svr(LogInfo *log_ifo, algo::PostInfo& post_ifo, std::string mediaDir, std::string& writer_path, std::string type) {
  int ret = 0;
  std::vector<bf::path> total_videos;
  std::string sub_str = "www/record/" + type + "/" + post_ifo.task_ifo.channelId;
  std::string video_dir = bf::path(mediaDir).append(sub_str).string();
  writer_path = bf::path(log_ifo->get_log_path(post_ifo.task_ifo.channelId)).append(std::to_string(post_ifo.time) + "_"+post_ifo.algo_type+".mp4").string();

  Json::Value root;
  root["video_dir"] = video_dir;
  root["start_time"] = std::to_string(post_ifo.start_time);
  root["end_time"] = std::to_string(post_ifo.end_time);
  root["writer_path"] = writer_path;
  
  std::string ret_body, msg = root.toStyledString();

  std::string http_svr = "http://127.0.0.1:9979";
  std::string uri = "/api/save_mp4";
  ret = post_data_base(ret_body, msg, http_svr, uri, -1, 1, log_ifo);
  if (log_ifo->log_level_1) spdlog::get("logger")->info("[ERROR]. save_video_by_svr ret:{}, ret_body:{}", ret, ret_body);
  if (ret != 0) { writer_path = ""; return ret;}

  Json::Value ret_json;
  ret = parse_json(ret_body, ret_json, false);
  if (ret != 0) { return ret;}

  std::string key = ret_json.get("key", Json::Value("")).asString();
  if (key == "") {return -1;}

  int64_t start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  while (true) {
    std::this_thread::sleep_for(milliseconds(10 * 1000));
    uri = "/api/query";
    Json::Value root;
    root["key"] = key;

    msg = root.toStyledString();
    ret = post_data_base(ret_body, msg, http_svr, uri, -1, 1, log_ifo);
    if (log_ifo->log_level_1) spdlog::get("logger")->info("[ERROR]. save_video_by_svr ret:{}, ret_body:{}", ret, ret_body);
    if (ret == 0) { spdlog::get("logger")->info("save_video_by_svr save_mp4 ok!");break;}

    int64_t cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    if (cur_time - start_time > 10 * 60 * 1000) {
      ret = -1; 
      spdlog::get("logger")->info("[ERROR]. save_video_by_svr timeout.");
      break;
    }
  }

  return ret;
}

int check_mp4_save_over(LogInfo *log_ifo, int64_t end_time, std::string mediaDir, std::string sub_str) {

  int64_t cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();;
  std::string ymd_str = time_int64_to_string(cur_time, "%Y-%m-%d");
  std::string today_dir = bf::path(mediaDir).append(sub_str).append(ymd_str).string();
  if (log_ifo->log_level_1) spdlog::get("logger")->info("today_dir: {}", today_dir);

  std::vector<bf::path> paths;
  std::string saveing_path;
  get_files_in_dir(today_dir, paths, ".mp4");
  if (log_ifo->log_level_1) spdlog::get("logger")->info("get_files_in_dir: {}", paths.size());
  if (paths.size() == 0) { return -1;}

  // 找正在写视频的文件 可能有多个
  int64_t last_time = 0;
  for (auto p : paths) {
    std::string filename = p.filename().string(); 
    if (filename.length() < 2) { 
      spdlog::get("logger")->info("ERROR. check_mp4_save_over: {}", p.string());
      continue;
    }
    if (filename.substr(0,1) == ".") {
      // 当视频写完时，此文件将不存在导致崩溃
      // time_t wtime = bf::last_write_time(p); 
      std::string time_str = p.parent_path().filename().string() + " " + filename.substr(1, 8);
      if (log_ifo->log_level_1) spdlog::get("logger")->info("time_str: {}", time_str);

      int64_t name_time = 1000 * (int64_t)convert_time_str2time_stamp(time_str, "%Y-%m-%d %H-%M-%S");
      if (name_time <= 0) {spdlog::get("logger")->info("ERROR. name_time: {}, last_time:{}", name_time, last_time);}
      if (log_ifo->log_level_1) spdlog::get("logger")->info("name_time: {}, last_time:{}", name_time, last_time);

      if (name_time > last_time) { last_time = name_time; saveing_path = p.string();}
    }
  }
  if (saveing_path.empty()) { return -1; }
  if (log_ifo->log_level_1) spdlog::get("logger")->info("last_time:{}, saveing_path: {}", last_time, saveing_path);

  int64_t mp4_time = last_time;
  if (log_ifo->log_level_1) spdlog::get("logger")->info("end_time: {}, mp4_time: {}", std::to_string(end_time), std::to_string(mp4_time));

  if (end_time < mp4_time){ return 0;}
  return -1;

}

// Saver::Saver(LogInfo* log_info):log_ifo(log_info){
Saver::Saver(){
}
Saver::~Saver() {
}

int Saver::open_mp4(algo::PostInfo& data_ifo,std::string file_name,int64_t video_paths_stime,int total_len_time, bool is_seek)
{
  int ret = 0;

  ret = avformat_open_input(&inputFormatContext, file_name.c_str(), NULL, NULL);
  if (ret != 0) {
      printf("Failed to open input file\n");
      avformat_close_input(&inputFormatContext);
      return ret;
  }
  ret = avformat_find_stream_info(inputFormatContext, NULL);
  if (ret < 0) {
      printf("Failed to find stream information\n");
      avformat_close_input(&inputFormatContext);
      return ret;
  }

  // 查找视频流索引
  videoStreamIndex = -1;
  for (int i = 0; i < inputFormatContext->nb_streams; i++) {
      if (inputFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
          videoStreamIndex = i;
          break;
      }
  }
  if (videoStreamIndex == -1) {
      // 处理找不到视频流的情况
      avformat_close_input(&inputFormatContext);
      return -1;
  }
  std::cout<<"videoStreamIndex:"<<videoStreamIndex<<std::endl;
  inputStream = inputFormatContext->streams[videoStreamIndex];
 
  if(left_durationInSeconds == 0 || is_seek){
    seekTarget = ((data_ifo.start_time - video_paths_stime) / 1000) * AV_TIME_BASE;
    // 从截取开始到视频结束的总时长 
    int seektoend_time = total_len_time - seekTarget/AV_TIME_BASE;
    int durationInSeconds  = (data_ifo.end_time - data_ifo.start_time)/1000;
    // std::cout<<"durationInSeconds1:"<<durationInSeconds<<std::endl;

    if(durationInSeconds > seektoend_time){
      left_durationInSeconds = durationInSeconds - seektoend_time;
      // durationInSeconds = seektoend_time;  //如果有第二个视频，那么就不需要
    }
    endTimestamp =((data_ifo.start_time - video_paths_stime) / 1000 + durationInSeconds) * AV_TIME_BASE;

    // std::cout<<"durationInSeconds:"<<durationInSeconds<<std::endl;
    // std::cout<<"seekTarget:"<<seekTarget<<std::endl;
    // std::cout<<"seektoend_time:"<<seektoend_time<<std::endl;
    std::cout<<"left_durationInSeconds:"<<left_durationInSeconds<<std::endl;
  }
  else{
    //暂时就是裁剪的2个视频 所以遗留的就是第二个视频的数据
    endTimestamp = left_durationInSeconds * AV_TIME_BASE;
  }
  if(is_seek){
    // 定位到指定的起始时间
    if (av_seek_frame(inputFormatContext, -1, seekTarget, AVSEEK_FLAG_BACKWARD) < 0) {
      fprintf(stderr, "无法定位到指定的起始时间\n");
      avformat_close_input(&inputFormatContext);
      return -1;
    }
  }
  std::cout<<"endTimestamp:"<<endTimestamp<<std::endl;

  std::cout<<"open ok !!!!!:"<<std::endl;

  return 0;
}

int Saver::set_out_mp4(std::string file_name)
{
  int ret = 0;
  ret = avformat_alloc_output_context2(&outputFormatContext, NULL, NULL, file_name.c_str());
  if (ret < 0) {
      printf("Failed to allocate output format context\n");
      avformat_free_context(outputFormatContext);
      return ret;
  }

  outputStream = avformat_new_stream(outputFormatContext, NULL);
  if (!outputStream) {
      printf("Failed to allocate output stream\n");
      avformat_free_context(outputFormatContext);
      return -1;
  }

  ret = avcodec_parameters_copy(outputStream->codecpar, inputStream->codecpar);
  if (ret < 0) {
      printf("Failed to copy codec parameters\n");
      avformat_free_context(outputFormatContext);
      return ret;
  }

  outputStream->time_base = inputStream->time_base;
  outputStream->codecpar->codec_tag = 0;
  
  ret = avio_open(&outputFormatContext->pb,file_name.c_str(), AVIO_FLAG_WRITE);
  if (ret < 0) {
      printf("Failed to open output file\n");
      std::cout<<"file_name:"<<file_name<<std::endl;
      avformat_free_context(outputFormatContext);
      return ret;
  }

  ret = avformat_write_header(outputFormatContext, NULL);
  if (ret < 0) {
      printf("Failed to write output file header\n");
      avformat_free_context(outputFormatContext);
      if (outputFormatContext && !(outputFormatContext->flags & AVFMT_NOFILE)){
        avio_closep(&outputFormatContext->pb);//avio打开要释放
      }
      return ret;
  }
  return 0;
}

int Saver::contact_two_mp4(std::string file_one,std::string file_two,std::string out_file){

  std::vector<std::string> fileList;
  fileList.push_back(file_one);
  fileList.push_back(file_two);

  //获得原始输入视频文件编码等信息
  const AVOutputFormat* ofmt = NULL;//输出格式
  AVFormatContext* ifmt_ctx = NULL, * ofmt_ctx = NULL;//视频数据维护对象
  AVPacket* pkt = NULL;//数据包

  int ret;//函数执行返回码
  int stream_index;//数据流索引
  //获得输出文件名
  const char* out_filename = out_file.c_str();
  int64_t i = 0;//用于计算时间戳，同时也是帧数
  int64_t p_max_dts = 0;//用于拼文件的时间戳

  pkt = av_packet_alloc();//初始化数据包结构
  if (!pkt){return -1;}

  if ((ret = avformat_open_input(&ifmt_ctx, file_one.c_str(), 0, 0) < 0)){
    std::cout<<"contact_two_mp4 open error!!"<<std::endl;
    goto end;//打开文件失败
  }

  //根据第一个文件获得其中的编码等参数，这里要求两个文件的编码格式一样就是因为在写入文件时用的是相同的配置没有进行转码等操作
  if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0){
    std::cout<<"contact_two_mp4 avformat_find_stream_info error!!"<<std::endl;
    goto end;
  }

  avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
  if (!ofmt_ctx){std::cout<<"contact_two_mp4 open out_file error!!"<<std::endl; goto end;}

  ofmt = ofmt_ctx->oformat;
  //查找视频流并复制视频流的参数到输出流
  for (int i = 0; i < ifmt_ctx->nb_streams; ++i){
    AVStream* in_stream = ifmt_ctx->streams[i];
    AVCodecParameters* in_codecpar = in_stream->codecpar;
    //非视频流跳过
    if (in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO){continue;}

    AVStream* out_stream = avformat_new_stream(ofmt_ctx, NULL);//创建输出流
    if (!out_stream){goto end;}

    ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);//复制解码器参数
    if (ret < 0){goto end;}

    out_stream->time_base = in_stream->time_base;//复制时间基
    stream_index = i;
    out_stream->codecpar->codec_tag = 0;
    break;
  }
  avformat_close_input(&ifmt_ctx);//关闭文件

  //打开输出文件
  if (!(ofmt->flags & AVFMT_NOFILE)){
    ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
    if (ret < 0){goto end;}
  }

  ret = avformat_write_header(ofmt_ctx, NULL);//写入头信息，如编码等内容
  if (ret < 0){goto end;}

  for (int index = 0; index < fileList.size(); ++index){ //遍历文件
    if ((ret = avformat_open_input(&ifmt_ctx, fileList[index].c_str(), 0, 0)) < 0){
      goto end;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0){ //查找文件流信息
      goto end;
    }
    
    //对流直接进行转写
    while (true){
      AVStream* in_stream, * out_stream;
      ret = av_read_frame(ifmt_ctx, pkt);
      if (ret < 0){break;}

      pkt->stream_index = stream_index;//视频流编号
      in_stream = ifmt_ctx->streams[stream_index];
      out_stream = ofmt_ctx->streams[stream_index];
      //这里要对时间戳进行处理，否则写入的时候会失败
      //单帧时长
      int64_t frameDuration = av_rescale_q(1, av_inv_q(in_stream->time_base), in_stream->r_frame_rate);
      //将单帧的时间从输入流转化到输出流时间
      int64_t _t = av_rescale_q(frameDuration, in_stream->time_base, out_stream->time_base);

      //计算时间戳，并进行累计以推算后面的时间戳
      p_max_dts = _t * (i);
      pkt->dts = p_max_dts;
      pkt->pts = pkt->dts;
  
      //pkt现在是空的，这个函数会获得pkt内容的所有权并重置，因此不需要unref，但是write_frame情况不同，需要手动释放
      ret = av_write_frame(ofmt_ctx, pkt);//直接将包写入输出文件不进行解码
      av_packet_unref(pkt);
      if (ret < 0){break;}
      ++i;
    }

    //关闭文件
    avformat_close_input(&ifmt_ctx);
  }

  av_write_trailer(ofmt_ctx);//写文件尾

end:
  av_packet_free(&pkt);//这里传指针，因为要将pkt设为null
  avformat_close_input(&ifmt_ctx);//同理
  if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE)){
    avio_closep(&ofmt_ctx->pb);//avio打开要释放
  }
  avformat_free_context(ofmt_ctx);
  if (ret < 0 && ret != AVERROR_EOF){return -1;}//异常结束
  return 0;

}

}