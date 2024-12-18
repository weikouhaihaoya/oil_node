#pragma once
#include <opencv2/opencv.hpp>
#include "base_util/utils.h"
#include "common/total_struct.h"
//#include "camera/error_code.h"

extern "C" {
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libavutil/imgutils.h>
  #include <libavutil/timestamp.h>
}

namespace common {

using namespace httplib;

int get_img_by_rtsp(std::string rtsp_url, cv::Mat& img);
int check_rtsp_is_ok(std::string rtsp_url);

int save_video_by_kit(LogInfo *log_ifo, algo::PostInfo& post_ifo, std::string mediaDir, std::string& writer_path, std::string type);

int save_video_by_nvr(LogInfo *log_ifo, algo::PostInfo& pi, NVRInfo nvr_ifo, std::string& writer_path);

int save_video_by_fmpg(LogInfo *log_ifo, algo::PostInfo& post_ifo, std::string mediaDir, std::string& writer_path, std::string type);

int check_mp4_save_over(LogInfo *log_ifo, int64 end_time, std::string mediaDir, std::string sub_str);

int save_video_by_svr(LogInfo *log_ifo, algo::PostInfo& post_ifo, std::string mediaDir, std::string& writer_path, std::string type);

  class Saver
  {
    public:
      Saver();
      ~Saver();
    public:
      int open_mp4(algo::PostInfo& data_ifo,std::string file_name,int64_t video_paths_stime,int total_len_time,bool is_seek);
      int set_out_mp4(std::string file_name);
      int contact_two_mp4(std::string file_one,std::string file_two,std::string out_file);

    public:
      AVStream* inputStream = nullptr;
      AVStream* outputStream = nullptr;
      AVFormatContext* inputFormatContext = NULL;
      AVFormatContext* outputFormatContext = NULL;
      int64_t endTimestamp = 0;
      int64_t seekTarget = 0;
      int videoStreamIndex = -1;
      int left_durationInSeconds = 0;  //在两个文件中截取视频时，截取到的第二个文件的持续时间。
  };

}   // namespace smartbox
