#include <algorithm>
#include <mutex>
#ifdef __x86_64__
#include <cpuid.h>
#endif
#ifdef _WIN32
#else
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h> 
#endif

#include "base_util/utils.h"
#include "base_util/error_code.h"

boost::random::mt19937 mt_gen;

std::string get_today_date(bool ret_recursive, int ydiff, int mdiff, int ddiff) {
  int64_t cur_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();;
    
  std::string today_str = "";
  if (ret_recursive) {
    today_str = time_int64_to_string(cur_time, "%Y/%m/%d");
  } else {
    today_str = time_int64_to_string(cur_time, "%Y-%m-%d-%H-%M-%S");
  }
  return today_str;
}

std::string randstr(int len) {
  // 随机种子最好不要轻易修改
  static int cnt = 0;
  if (cnt == 0) {
    cnt+=1;
    srand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  }
  
  std::string str(len, '0');
  int i;
  for (i = 0; i < len; ++i)
  {
    switch ((rand() % 3)) {
    case 1:
      str[i] = 'A' + rand() % 26;
      break;
    case 2:
      str[i] = 'a' + rand() % 26;
      break;
    default:
      str[i] = '0' + rand() % 10;
      break;
    }
  }
  return str;
}

int get_dir_recurrsive(std::string root, int level, std::vector< std::pair<int, std::string > > &all_files)
{
  int ret_code = 0;
  bf::directory_iterator itEnd;
  try
  {
    bool flags = true;
    for (bf::directory_iterator it(root); it != itEnd;)
    {
      if (bf::is_directory(it->path())) {
        flags = false;
        get_dir_recurrsive(it->path().string(), level + 1, all_files);
        //std::pair<int, std::string> one_path(it.level(), it->path().string());
        //all_files.push_back(one_path);
      }
      try { ++it; }
      catch (const bf::filesystem_error& ex)
      {
        break;
      }
    }
    if (flags) {
      std::pair<int, std::string > temp(level, root);
      all_files.push_back(temp);
    }
  }
  catch (const std::exception& ex)
  {
    ret_code = -1;
    //spdlog::get("logger")->info("************* exception *****************\n");
    //spdlog::get("logger")->info(ex.what() << '\n');
  }
  return ret_code;
}


int get_all_files(std::string root, std::vector< std::pair<int, std::string > > all_files)
{
  int ret_code = 0;
  bf::recursive_directory_iterator itEnd;
  try
  {
    for (bf::recursive_directory_iterator it(root); it != itEnd;)
    {
      if (bf::is_regular_file(it->path())) {
        std::pair<int, std::string> one_path(it.level(), it->path().string());
        all_files.push_back(one_path);
      }
      try { ++it; }
      catch (const bf::filesystem_error& ex)
      {
        break;
      }
    }
  }
  catch (const std::exception& ex)
  {
    ret_code = -1;
    //spdlog::get("logger")->info("************* exception *****************\n");
    //spdlog::get("logger")->info(ex.what() << '\n');
  }
  return ret_code;
}


int get_files_in_dir_recurrsive(std::string root, int level, std::vector< std::pair<int, std::string > > &all_files)
{
  int ret_code = 0;
  bf::directory_iterator itEnd;
  try
  {
    for (bf::directory_iterator it(root); it != itEnd;)
    {
      if (bf::is_directory(it->path())) {
        get_files_in_dir_recurrsive(it->path().string(), level + 1, all_files);
        //std::pair<int, std::string> one_path(it.level(), it->path().string());
        //all_files.push_back(one_path);
      }
      else if (bf::is_regular_file(it->path())) {
        all_files.push_back(std::make_pair(level, it->path().string()));
      }
      try { ++it; }
      catch (const bf::filesystem_error& ex)
      {
        break;
      }
    }
  }
  catch (const std::exception& ex)
  {
    ret_code = -1;
    //spdlog::get("logger")->info("************* exception *****************\n");
    //spdlog::get("logger")->info(ex.what() << '\n');
  }
  return ret_code;
}

int get_last_dirs(std::string dir, std::vector< bf::path > &all_files, int level)
{
  int ret = 0;
  bf::directory_iterator it_end;
  try {
    std::vector< bf::path > cur_files;
    for (bf::directory_iterator it(dir); it != it_end;) {
      if (bf::is_directory(it->path()) ) {
        if (level > 1 || level< 0) {
          ret = get_last_dirs(it->path().string(), all_files, level-1);
        } else {
          cur_files.push_back(it->path());
        }
      }
      try { ++it; }
      catch (const bf::filesystem_error& ex) { break; }
    }
    if (cur_files.size() == 0) { cur_files.push_back(bf::path(dir)); }
    all_files.insert(all_files.begin(), cur_files.begin(), cur_files.end());
  }
  catch (const std::exception& ex) { }
  return ret;
}

int get_files_in_dir(std::string dir,
                                 std::vector< bf::path > &all_files,
                                 std::string exten)
{
    int ret_code = 0;
    bf::recursive_directory_iterator itEnd;
    try
    {
        for (bf::recursive_directory_iterator it(dir); it != itEnd;)
        {
            if (bf::is_regular_file(it->path()) && (exten == "" || boost::algorithm::ends_with(it->path().string(), exten)) ) {
                all_files.push_back(it->path());
            }
            try { ++it; }
            catch (const bf::filesystem_error& ex)
            {
                break;
            }
        }
    }
    catch (const std::exception& ex)
    {

    }
    return ret_code;
}


int get_files_in_dir(std::string dir, std::vector< std::string > &all_files)
{
  int ret_code = 0;
  bf::directory_iterator itEnd;
  try
  {
    for (bf::directory_iterator it(dir); it != itEnd;)
    {
      if (bf::is_regular_file(it->path())) {
        all_files.push_back(it->path().string());
      }
      try { ++it; }
      catch (const bf::filesystem_error& ex)
      {
        /*spdlog::get("logger")->info("************* filesystem_error *****************\n");
        spdlog::get("logger")->info(ex.what() << '\n');*/
        ret_code = -1;
        break;
      }
    }
  }
  catch (const std::exception& ex)
  {
    ret_code = -1;
    //spdlog::get("logger")->info("************* exception *****************\n");
    //spdlog::get("logger")->info(ex.what() << '\n');
  }
  return ret_code;
}

int return_error(const int code,const std::string &msg, std::string &error_msg)
{
  Json::Value root;
  root["code"] = code;
  root["msg"] = msg;
  error_msg = root.toStyledString();
  return code;
}

int get_CPU_core_num()
{
  #if defined(_WIN32)
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
  #elif defined(LINUX) || defined(SOLARIS) || defined(AIX)
    return get_nprocs();   //GNU fuction
  #else
    return NOT_IMPLEMENT_ERROR;
  #endif
}

std::string get_cpu_adress_x86_64() {
  std::string serial_number = "";
#ifdef __x86_64__
  unsigned int level = 1;
  unsigned eax = 3 /* processor serial number */, ebx = 0, ecx = 0, edx = 0;
  __get_cpuid(level, &eax, &ebx, &ecx, &edx);
  // byte swap
  int first = ((eax >> 24) & 0xff) | ((eax << 8) & 0xff0000) | ((eax >> 8) & 0xff00) | ((eax << 24) & 0xff000000);
  int last = ((edx >> 24) & 0xff) | ((edx << 8) & 0xff0000) | ((edx >> 8) & 0xff00) | ((edx << 24) & 0xff000000);
  // tranfer to string
  std::stringstream ss;
  ss << std::hex << first;
  ss << std::hex << last;
  ss >> serial_number;
#endif
  return serial_number;
}


#ifdef _WIN32
#else
std::string get_home_path()
{
//     if ( !path.empty() and path[0] == '~') {
//     assert(path.size() == 1 or path[1] == '/');  // or other error handling
//     char const* home = getenv("HOME");
//     if (home or ((home = getenv("USERPROFILE"))) {
//       path.replace(0, 1, home);
//     }
//     else {
//       char const *hdrive = getenv("HOMEDRIVE"),
//         *hpath = getenv("HOMEPATH");
//       assert(hdrive);  // or other error handling
//       assert(hpath);
//       path.replace(0, 1, std::string(hdrive) + hpath);
//     }
//   }
    char* home_path = getenv("HOME");
    return home_path;
}

std::string get_linux_ip() {
  std::string hostName;
  std::string Ip;
  char name[256];
  gethostname(name, sizeof(name));
  hostName = name;
  
  struct hostent* host = gethostbyname(name);
  char ipStr[32];
  const char* ret = inet_ntop(host->h_addrtype, host->h_addr_list[0], ipStr, sizeof(ipStr));
  if (NULL==ret) {
    std::cout << "hostname transform to ip failed";
    return "";
  }
  Ip = ipStr;
  return Ip;
}

void executeCMD(const char *cmd, std::string& res) {
  char buf_ps[1024];
  char ps[1024]={0};
  char result[65536];
  FILE *ptr;
  strcpy(ps, cmd);
  if((ptr=popen(ps, "r"))!=NULL) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(fgets(buf_ps, 1024, ptr)!=NULL)
    {
      strcat(result, buf_ps);
      // std::cout << "result: " << strlen(result) << std::endl;
      //  if(strlen(result)>1024)
      //      break;
    }
    pclose(ptr);
    ptr = NULL;
  }
  else {
    printf("popen %s error\n", ps);
  }
  res = std::string(result);
  
  memset(result, 0, sizeof(result));
  memset(buf_ps, 0, sizeof(buf_ps));
  memset(ps, 0, sizeof(ps));
}

int monitor_process_online(std::string app_name) {
  std::string cmd = "pgrep " + app_name;
  std::string res;
  executeCMD(cmd.c_str(), res);
  // spdlog::get("logger")->info("executeCMD: {}", res);
  if (res.length() < 1 ) { return -1;}
  return 0;
}

float monitor_device_online(std::string device_ip) {
  std::string cmd = "ping -c 4 " + device_ip;
  std::string res;
  executeCMD(cmd.c_str(), res);

  spdlog::get("logger")->info("executeCMD: {}", res);

  std::vector<std::string> lines;
  boost::algorithm::split(lines, res, boost::is_any_of("\n"), boost::token_compress_on);
  spdlog::get("logger")->info("lines: {}", lines.size());

  float delay_total = 0.;
  for (auto& line : lines) {
    if (line.substr(0, 8) != std::string("64 bytes")) { continue; }
    std::vector<std::string> steps;
    boost::algorithm::split(steps, line, boost::is_any_of(" "), boost::token_compress_on);
    for (auto& step : steps) { 
      if (step.substr(0, 5) != std::string("time=")) { continue; }
      delay_total += std::stof(step.substr(5));
    }
  }
  if (delay_total < 0.01) { return -1.; }

  return delay_total / 4;
}
#endif

int check_camera_connectivity(std::string camera_ip) {
  httplib::Headers header = {
    {"Accept", "text/html, application/xhtml+xml, */*"},
    {"Accept-Language", "zh-CN"},
    {"User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko"},
    {"Accept-Encoding", "gzip, deflate"},
    {"Connection", "Keep-Alive"}
  };

  std::vector<std::string> schemes = {"http://", "https://"};
  std::string uri = "/";

  int ret = -1;
  for (auto scheme : schemes) {
    httplib::Client cli = httplib::Client((scheme+camera_ip).c_str());
    cli.set_connection_timeout(2, 0);
    cli.set_read_timeout(2, 0); 
    cli.set_write_timeout(2, 0); 

    auto res = cli.Get(uri.c_str(), header);
    if (!res) { ret = -1; }
    else {
      httplib::Headers::iterator it = res->headers.find("Date");
      if (it != res->headers.end()) {
        std::cout << "time_str_gmt: " << it->second << std::endl;
      }
      ret = 0;
      break;
    }
  }
  return ret;
}

int expiration_date_online(const std::string &end_time)
{
  int ret_code  = 0;
  std::string server_string_json;
  if ((ret_code = get_server_time(server_string_json)) != 200){return ret_code;}
  //spdlog::get("logger")->info("sss" <<server_string_json);
  Json::Value result;
  Json::Reader reader;

  if (!reader.parse(server_string_json, result)){
      return json_read_parse_error;
  }

  std::string server_string = result["time"].asString();

  boost::posix_time::ptime server_time = boost::posix_time::time_from_string(server_string);

  // boost::posix_time::ptime local_time = boost::posix_time::second_clock::local_time();
  
  // date today = day_clock::local_day();
  boost::posix_time::ptime end_ptime  = boost::posix_time::time_from_string(end_time);
  if(end_ptime < server_time ){
    return APP_EXPIRED_ERROR;   // alarm
  }
  return 0;
}

int get_server_time(std::string &output_info)
{
   using namespace httplib;
   Client cli("0.0.0.0", 8000);

   Params params;
   params.emplace("get_time", "1");

   auto res = cli.Post("/get_time", params);

   if (res){
       output_info = res->body;
    //    spdlog::get("logger")->info("output_info: " << output_info);
       return res->status;
   }
   //spdlog::get("logger")->info("res->body: " << res->body);
   // spdlog::get("logger")->info("res->status: " << res->status);
   return HTTP_UNKNOW_ERROR;
}

#ifdef _WIN32
time_t convert_time_str2time_stamp(std::string time_str, const char * format){
  struct tm tm_;
  int year, month, day, hour, minute,second;
  sscanf(time_str.c_str(),"%d-%d-%d %d-%d-%d", &year, &month, &day, &hour, &minute, &second);
  tm_.tm_year  = year-1900;
  tm_.tm_mon   = month-1;
  tm_.tm_mday  = day;
  tm_.tm_hour  = hour;
  tm_.tm_min   = minute;
  tm_.tm_sec   = second;
  tm_.tm_isdst = 0;

  time_t timeStamp = mktime(&tm_);
  return timeStamp;
}
#else
time_t convert_time_str2time_stamp(std::string time_str, const char * format){
  struct tm timeinfo;
  // "%Y-%m-%d %H-%M-%S"
  strptime(time_str.c_str(), format,  &timeinfo);
  // 必须设置为-1 不然某些架构下无法正确执行
  timeinfo.tm_isdst = -1; 
  time_t timeStamp = mktime(&timeinfo);
  return timeStamp;
}
#endif

std::string time_int64_to_string(int64_t timepoint, const char * format) {
  timepoint /= 1000;
  char now[64];
  time_t tt = timepoint;
  struct tm *ttime; 
  ttime = localtime(&tt);
  // "%Y-%m-%d %H:%M:%S"
  strftime(now,64,format,ttime); 
  return std::string(now);
}


void split_utf8_string(std::string str, std::vector<std::string>& list) {
  size_t strSize = str.size();
  int i = 0;

  while (i < strSize) {
    int len = 1;
    for (int j = 0; j < 6 && (str[i] & (0x80 >> j)); j++) {
      len = j+1;
    }
    list.push_back(str.substr(i, len));
    i += len;
  }
}

bool string_contains(std::string &string, std::string c) {
  std::vector<std::string> list;
  split_utf8_string(string, list);
    for (auto s : list){ if (s == c){return true;} }
    return false;
}



int md5(std::string &str_md5, const char * const buffer, size_t buffer_size, int flag) {
  if(buffer == nullptr) { return -1; }

  boost::uuids::detail::md5 boost_md5;
  boost_md5.process_bytes(buffer, buffer_size);
  boost::uuids::detail::md5::digest_type digest;
  boost_md5.get_digest(digest);
  const auto char_digest = reinterpret_cast<const char*>(&digest);
  str_md5.clear();
  boost::algorithm::hex(char_digest,char_digest+sizeof(boost::uuids::detail::md5::digest_type), std::back_inserter(str_md5));

  if (flag == 1) {boost::algorithm::to_lower(str_md5);}
  return 0;
}

int parse_json(const std::string& str, Json::Value& root, bool read_file) {
  Json::Reader reader;
  if (read_file) {
    std::ifstream ifs;
    ifs.open(str);
    if (!ifs.is_open()) { return file_not_found_error; }
    if (!reader.parse(ifs, root)) { return json_read_parse_error; }
  } else {
    if (!reader.parse(str, root)) { return json_read_parse_error; }
  }
  return 0;
}


void remove_file(std::string data_dir, time_t keep_seconds, std::string sufix) {
  std::vector<bf::path> total_files;
  get_files_in_dir(data_dir, total_files, sufix); 

  // 秒
  time_t nowtime; time(&nowtime);
  
  for (auto& p : total_files) {
    // 不对带.文件进行操作 可能引发异常
    std::string filename = p.filename().string(); 
    if (filename.length() < 2) { 
      spdlog::get("logger")->info("ERROR. remove_file: {}", p.string());
      continue; 
    }
    if (filename.substr(0,1) == ".") {continue;}
    if (!bf::exists(p)) { continue; }
    try {
      time_t write_time = bf::last_write_time(p);
      if (nowtime - write_time < keep_seconds) { continue; }
      bf::remove(p);
    } catch (...) {
      spdlog::get("logger")->info("ERROR. remove_file: {}", p.string());
    }
    // std::cout << "nowtime: " << nowtime << "  write_time: " << write_time << "  keep_seconds: " << keep_seconds << std::endl;
  }
}

void remove_history_img(std::string data_dir, int64_t time_point, int keep_day) {
  std::vector<bf::path> total_files;
  get_files_in_dir(data_dir, total_files, ""); 

  time_t nowtime; time(&nowtime);
  
  // std::sort(total_files.begin(), total_files.end(), [](bf::path& p1, bf::path& p2){return bf::last_write_time(p1) > bf::last_write_time(p2);});
  int txt_cnt = 0;
  for (auto& p : total_files) {
    if (!bf::exists(p)) { continue; }
    time_t f_time = bf::last_write_time(p);
    if (p.extension().string() != ".txt" && nowtime - f_time < keep_day * 24 * 3600) { continue; }
    // 日志只保留15天/15个s
    if (p.extension().string() == ".txt") { 
      txt_cnt++;
      if (nowtime - f_time < 15 * 24 * 3600 || txt_cnt < 15) { continue; } 
    }
    bf::remove(p);

    // std::stringstream ss(time_str.substr(1));
    // ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    // std::chrono::system_clock::time_point cur_time;
    // cur_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    // int64 cur_point = std::chrono::duration_cast<std::chrono::milliseconds>(cur_time.time_since_epoch()).count();
    // if (time_point - cur_point < keep_day * 24 * 3600 * 1000) { continue; }
    // bf::remove(p);
  }

  // tar yestorday img
  struct tm* p;
  nowtime -= 24 * 3600;
  p = localtime(&nowtime);

  std::string sub_str = "";
  sub_str = boost::filesystem::path(std::to_string(p->tm_year + 1900)).append(std::to_string(p->tm_mon + 1)).append(std::to_string(p->tm_mday)).string();
  std::string yesterday_dir = bf::path(data_dir).append(sub_str).string();
  spdlog::get("logger")->info("yesterday_dir: {}", yesterday_dir);

  // if (!bf::exists(yesterday_dir)) { return ;}
  // std::vector<bf::path> files;
  // get_files_in_dir(yesterday_dir, files, "train.jpg");
  // if (bf::exists(bf::path(yesterday_dir).append("1.tar"))) { return;}
  // tar_file(bf::path(yesterday_dir).append("1.tar").string(), files);
}

// int tar_file(std::string tar_path, std::vector<bf::path> files) {
//   int ret = -1;
//   TAR* ptar_handle = NULL;

//   // tar
//   ret = tar_open(&ptar_handle, tar_path.c_str(), NULL,  O_WRONLY | O_CREAT,  0644,  TAR_GNU);
//   if (-1 == ret)
//   {
//     spdlog::get("logger")->info("tar_open failed\ntar_path: {}, \nreason: {%s}\n", tar_path, strerror(errno));
//     return ret;
//   }
  
//   for (auto f : files) {
//     tar_append_file(ptar_handle, f.string().c_str(),  f.string().c_str());
//   }
//   tar_append_eof(ptar_handle);
//   tar_close(ptar_handle);

//   // //untar
//   // char* ptar_fname = "tartest.tar";
//   // char* savefdir = "test";
//   // iret = tar_open(&ptar_handle, ptar_fname, NULL,  O_RDONLY,  0644,  TAR_GNU);
//   // if (-1 == iret)
//   // {
//   //     printf("tar_open failed, reason: %s\n", strerror(errno));
//   //     exit(-1);
//   // }
//   // tar_extract_all(ptar_handle, savefdir);
//   // tar_close(ptar_handle);

//   return 0;

// }


int get_disk_percent(DiskInfo& du) {
  // /dev/mmcblk0p1   59G   28G   29G  49% /

  std::string path = "disk.txt";
  std::string point = "/";
  system(std::string("df -h > "+path).c_str());
  
  std::ifstream in(path);
  std::string line, tmp;
  if (!in) {
    spdlog::get("logger")->info("no such file: {}", path);
    return -1;
  }
  std::vector<std::string> data;
  while (getline(in, line)) {
    std::stringstream ss(line);
    std::string s1, s2, s3, s4, s5, s6;
    ss >> s1 >> s2 >> s3 >> s4 >> s5 >> s6;
    if (s6 == point) {
      data = {s2, s3, s4, s5};
      break;
    }
  }
  in.close();
  if (data.size() != 4) { return -1;}
  du.set(data);
  return 0;
}


std::string get_machine_ip() {
#ifdef _WIN32
  return "";
#else
  std::string command = "ifconfig"; 
  std::string res;
  executeCMD(command.c_str(), res);

  std::vector<std::string> lines;
  boost::algorithm::split(lines, res, boost::is_any_of("\n"), boost::token_compress_on);
  if (lines.size() == 0) {
    spdlog::get("logger")->info("executeCMD error.");
    return "";
  }

  std::string tmp,ret_ip;
  for (auto& line : lines) {
    std::stringstream ss(line);
    ss >> tmp;
    if (tmp == std::string("inet")) {
      ss >> ret_ip;
      // 特殊情况: inet addr:192.168.0.10  Bcast:192.168.0.255  Mask:255.255.255.0
      if (ret_ip.length() >= 5 && ret_ip.substr(0, 5) == std::string("addr:")) {ret_ip = ret_ip.substr(5);}

      if (ret_ip.length() >= 7){
        std::vector<std::string> ips;
        boost::algorithm::split(ips, ret_ip, boost::is_any_of("."), boost::token_compress_on);
        if (ips.size() == 4 && ips.back() != "1") {
          break;
        }
      }
    }
  }
  return ret_ip;
#endif
}

float get_board_thermal(CheckDevice device) {
  std::string path = "";
  if (device == check_orangepi) {
    path = "/sys/class/thermal/thermal_zone0/temp";
  } else {
    return 0.;
  }

  std::ifstream in(path);
  if (!in) {
    std::cout << "no such file: " << path << std::endl;
    return 0.;
  }
  std::string thermal = "0";
  std::getline(in, thermal);
  in.close();

  return std::stoi(thermal) * 1.0 / 1000;
}

int post_data_base(std::string& ret_body, std::string msg, std::string httpServer, std::string uri, int port, int isPost, LogInfo *log_ifo, httplib::Headers headers) {
  httplib::Client* cli;
  if (port < 0) { cli = new httplib::Client(httpServer.c_str()); }
  else { cli = new httplib::Client(httpServer.c_str(), port); }

  cli->set_connection_timeout(2, 0);
  cli->set_read_timeout(5, 0); 
  cli->set_write_timeout(1, 0); 
  
  auto res = isPost ? cli->Post(uri.c_str(), headers, msg, "application/json") : cli->Get(uri.c_str());
  if (!res || res->status != 200) {
  spdlog::get("logger")->info("ERROR. post_data_base cli.Post error. \nhttpServer:{} \nuri:{} \nmsg:{} \nhttp code: {}\n", httpServer, uri, msg, (res ? res->status : int(res.error())));
    delete cli; return -1;
  }
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("post_data_base: uri:{}, res->body: {}", uri, res->body);

  ret_body = res->body;
  Json::Value ret_json;
  int ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. post_data_base {} parse_json ret || ret_json[\"code\"]: {}", uri, ret);
    delete cli; return ret;
  }
  int ret_code = ret_json.get("code", Json::Value(int(HTTP_KEY_FAULT_ERROR))).asInt();
  if (ret_code != 0 && ret_code != 200) {
    spdlog::get("logger")->info("ERROR. post_data_base {} parse_json ret || ret_json[\"code\"]: {}", uri, ret_code);
    delete cli; return ret_code;
  }
  
  delete cli; return 0;
}

int get_data_base(std::string& ret_body, httplib::Params params, std::string httpServer, std::string uri, int port, int isPost, LogInfo *log_ifo) {
  httplib::Client* cli;
  if (port < 0) { cli = new httplib::Client(httpServer.c_str()); }
  else { cli = new httplib::Client(httpServer.c_str(), port); }

  cli->set_connection_timeout(2, 0);
  cli->set_read_timeout(5, 0); 
  cli->set_write_timeout(1, 0); 

  httplib::Headers headers = {
    {"Content-type", "application/json;charset=utf-8" }
  };
  auto res = cli->Get(uri.c_str(), params, headers);
  if (!res || res->status != 200) {
  spdlog::get("logger")->info("ERROR. get_data_base cli.Post error. \nhttpServer:{} \nuri:{} \nmsg:{} \nhttp code: {}\n", httpServer, uri, "httplib::Params", (res ? res->status : int(res.error())));
    delete cli; return -1;
  }
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("{}: res->body: {}", uri, res->body);

  ret_body = res->body;
  Json::Value ret_json;
  int ret = parse_json(res->body, ret_json, false);
  if (ret != 0 || !ret_json.isMember("code") || (ret_json["code"].asInt() != 0 && ret_json["code"].asInt() != 200)) { 
      spdlog::get("logger")->info("ERROR. get_data_base {} parse_json ret || ret_json[\"code\"]: {}", uri, (ret != 0 ? ret : ret_json["code"].asInt()));
      delete cli; return -1;  
  }
  delete cli; return 0;
}

int post_file(std::string& url, std::string httpServer, std::string uri, std::string writer_path, std::string key_name, int read_time, int write_time, LogInfo *log_ifo) {
  if (uri.empty() || httpServer.empty()) { return -1;}
  httplib::Client cli(httpServer.c_str());
  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(read_time, 0); 
  cli.set_write_timeout(write_time, 0); 
  std::string content; 
  std::ifstream infile;
  infile.open(writer_path);
  infile.seekg(0,std::ios::end);//跳转到文件末尾
  long len = infile.tellg();//获取偏移量-文件长度
  infile.seekg(0,std::ios::beg);//跳转到文件起始位置
  content.resize(len);//申请空间
  infile.read(&(content)[0],len);//读取文件数据
  infile.close();//关闭文件
  if (log_ifo && log_ifo->log_level_1) spdlog::get("logger")->info("post_file content: {}", content.length());
  if (log_ifo && log_ifo->log_level_2) {
    spdlog::get("logger")->info("post_file  uploadFile uri: {}", uri);
    spdlog::get("logger")->info("post_file httpServer: {}", httpServer);
  }

  httplib::MultipartFormDataItems items = {
    { "file", content, bf::path(writer_path).filename().string(), "application/octet-stream" },
  };
  auto res = cli.Post(uri.c_str(), items);
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("post_file cli.Post error. http code: {}", res ? res->status : int(res.error()));
    return -1;
  }
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("post_file cli.Post get data: \n{}", res->body);

  Json::Value ret_json;
  int ret = parse_json(res->body, ret_json, false);
  if (ret != 0) { 
    spdlog::get("logger")->info("ERROR. post_data_base {} parse_json ret || ret_json[\"code\"]: {}", uri, ret);
    return -1;  
  }

  int ret_code = ret_json.get("code", Json::Value(-1)).asInt();
  if (ret_code != 0 && ret_code != 200) {
    spdlog::get("logger")->info("ERROR. post_data_base {} parse_json code: {}", uri, ret_code);
    return -1;  
  }
  
  // 先判断是否在list里面
  if (ret_json.isMember("data")) {
    Json::Value item;
    if (ret_json["data"].isArray() && ret_json["data"].size() > 0) {
      item = ret_json["data"][0];
    }
    else if (ret_json["data"].isObject()) {
      item = ret_json["data"];
    }
    if (item.isMember(key_name)) { url = item[key_name].asString(); }
  }
  else if (ret_json.isMember(key_name)) { url = ret_json[key_name].asString(); }

  if (log_ifo && log_ifo->log_level_1) spdlog::get("logger")->info("post_file url: {}", url);
  return 0; 
}

int download_file(std::string updateUrl, std::string httpServer, std::string saveDir,LogInfo *log_ifo) {
  // 下载更新包
  int ret = 0;
  if (httpServer.empty() && updateUrl.length() > 7) {
    httpServer = updateUrl.substr(0, updateUrl.find("/", 8));
  }
  if (httpServer.empty()) {return HTTP_PARAM_ERROR;}
  auto cli = httplib::Client(httpServer.c_str()); 

  cli.set_connection_timeout(2, 0);
  cli.set_read_timeout(5, 0); 
  cli.set_write_timeout(1, 0);

  std::string body;
  auto res = cli.Get(updateUrl.c_str(),
    [&](const char *data, size_t data_length) {
      body.append(data, data_length);
      return true;
  });
  if (!res || res->status != 200) {
    spdlog::get("logger")->info("post_file cli.Post error. http code: {}", res ? res->status : int(res.error()));
    return -1;
  }

  if (bf::is_directory(saveDir)) {saveDir = bf::path(saveDir).append(bf::path(updateUrl).filename().string()).string();}
  
  if (log_ifo && log_ifo->log_level_2) spdlog::get("logger")->info("download_file. save_path: {}", saveDir);

  std::fstream fout(saveDir , std::ios::binary | std::ios::out );
  fout.write(body.c_str(), body.length());
  fout.close();
  return ret;
}

void splitUrl(const std::string& url, std::string& host, std::string& path)
{
    size_t pos = url.find("://");
    if (pos != std::string::npos) {
        pos += 3; // 跳过 "://"
    } else {
        pos = 0;
    }
    size_t slashPos = url.find('/', pos);
    if (slashPos != std::string::npos) {
        host = url.substr(pos, slashPos - pos);
        path = url.substr(slashPos);
    } else {
        host = url.substr(pos);
        path = "/";
    }
}
