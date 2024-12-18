#include "zip_tool.h"

#ifdef _WIN32
#include <codecvt>
#endif

namespace common {


int zip_file(std::string tar_path, std::vector<bf::path> files) {
  // int ret = -1;
  // TAR* ptar_handle = NULL;

  // // tar
  // ret = tar_open(&ptar_handle, tar_path.c_str(), NULL,  O_WRONLY | O_CREAT,  0644,  TAR_GNU);
  // if (-1 == ret)
  // {
  //   spdlog::get("logger")->info("tar_open failed\ntar_path: {}, \nreason: {%s}\n", tar_path, strerror(errno));
  //   return ret;
  // }
  
  // for (auto f : files) {
  //   tar_append_file(ptar_handle, f.string().c_str(),  f.string().c_str());
  // }
  // tar_append_eof(ptar_handle);
  // tar_close(ptar_handle);

  // // //untar
  // // char* ptar_fname = "tartest.tar";
  // // char* savefdir = "test";
  // // iret = tar_open(&ptar_handle, ptar_fname, NULL,  O_RDONLY,  0644,  TAR_GNU);
  // // if (-1 == iret)
  // // {
  // //     printf("tar_open failed, reason: %s\n", strerror(errno));
  // //     exit(-1);
  // // }
  // // tar_extract_all(ptar_handle, savefdir);
  // // tar_close(ptar_handle);

  return 0;

}


int unzip_current_file(unzFile uf, std::string destFolder){
  char szFilePath[512];
  unz_file_info64 FileInfo;
  if (unzGetCurrentFileInfo64(uf, &FileInfo, szFilePath, sizeof(szFilePath), NULL, 0, NULL, 0) != UNZ_OK) {return false;}
 
  size_t len = strlen(szFilePath);
  if (len <= 0) { return false; }
 
  std::string save_path = destFolder;
  save_path = bf::path(save_path).append(szFilePath).string();
  if (szFilePath[len - 1] == '\\' || szFilePath[len - 1] == '/') {
    bf::create_directories(save_path.c_str());
    return true;
  }
  auto file = fopen(save_path.c_str(), "wb");
  if (file == nullptr) { return false; }
 
  const int BUFFER_SIZE = 4096;
  unsigned char byBuffer[BUFFER_SIZE];
  if (unzOpenCurrentFile(uf) != UNZ_OK) {
    fclose(file);
    return false;
  }
 
  while (true) {
    int nSize = unzReadCurrentFile(uf, byBuffer, BUFFER_SIZE);
 
    if (nSize < 0) {
      unzCloseCurrentFile(uf);
      fclose(file);
      return false;
    }
    else if (nSize == 0) { break; }
    else {
      size_t wSize = fwrite(byBuffer, 1, nSize, file);
      if (wSize != nSize) {
        unzCloseCurrentFile(uf);
        fclose(file);
        return false;
      }
    }
  }
 
  unzCloseCurrentFile(uf);
  fclose(file);
  return true;
}
 
int unzip_file(std::string zip_path, std::string unzip_dir){

  unzFile uf = unzOpen64(zip_path.c_str());
  if (uf == NULL) {
    spdlog::get("logger")->info("unzipFile error: {}", zip_path);
    return -1;
  } 
 
  unz_global_info64 gi;
  if (unzGetGlobalInfo64(uf, &gi) != UNZ_OK) {
    unzClose(uf);
    spdlog::get("logger")->info("unzipFile error: unzGetGlobalInfo64");
    return -1;
  }

  std::string path=zip_path;
  auto pos = path.find_last_of("/\\");
  if (pos != std::string::npos)
    path.erase(path.begin() + pos, path.end());
 
  for (int i = 0; i < gi.number_entry; ++i) {
    if (!unzip_current_file(uf, unzip_dir.c_str())) {
      unzClose(uf);
      spdlog::get("logger")->info("unzipFile unzipCurrentFile idx:{} error.", i);
      return -1;
    }
    if (i < gi.number_entry - 1) {
      if (unzGoToNextFile(uf) != UNZ_OK) {
        unzClose(uf);
        spdlog::get("logger")->info("unzipFile unzGoToNextFile idx:{} error.", i);
        return -1;
      }
    }
  }
  unzClose(uf);
  return 0;
}
 


}   // namespace monitor