#pragma once
#include <string>
#include <map>
#include <cassert>
 
class MyError {
public:
MyError(int value, const std::string& str) {
    m_value = value;
    m_message = str;
    // #ifdef _DEBUG
    // ErrorMap::iterator found = GetErrorMap().find(value);
    // if (found != GetErrorMap().end())
    // assert(found->second == m_message);
    // #endif
    GetErrorMap()[m_value] = m_message;
}

operator int() { return m_value; }

private:
int m_value;
std::string m_message;

typedef std::map<int, std::string> ErrorMap;
static ErrorMap& GetErrorMap() {
    static ErrorMap errMap;
    return errMap;
}

public:
static std::string GetErrorString(int value) {
    ErrorMap::iterator found = GetErrorMap().find(value);
    if (found == GetErrorMap().end()) { return "error";} // assert(false);
    return found->second;
}

};

static MyError RETURN_OK(0, "ok"); 
static MyError RETURN_OK200(200, "ok"); 
static MyError NOT_IMPLEMENT_ERROR(110, "未实现错误"); 

static MyError HTTP_UNKNOW_ERROR(1500, "http未知错误"); 
static MyError HTTP_KEY_ERROR(1501, "http参数KEY错误"); 
static MyError HTTP_RET_DATA_ERROR(1503, "http 返回数据错误"); 
static MyError HTTP_DOWNLOAD_DATA_ERROR(1506, "http 下载数据错误"); 
static MyError HTTP_PARAM_ERROR(1508, "http参数PARAM错误"); 
static MyError HTTP_KEY_FAULT_ERROR(1510, "http参数KEY缺失\"code\"错误"); 

static MyError ERROR_CODE_UNKNOW_ERROR(1550, "错误码未知"); 
static MyError APP_EXPIRED_ERROR(1600, "已过期"); 


// filesystem code
enum FILESYSTEM_ERROR{
    file_not_found_error = 400,
    file_not_exist_error = 450,

};


// Json error
enum JSON_ERROR{
    json_read_parse_error=501,
    json_key_error=502,
    json_value_error=503,
    json_not_a_array_error= 560,
    json_size_is_zero_error= 561,
};

// image error
enum IMAGE_ERROR{
    IMAGE_EMPTY_ERROR= 810,
    IMAGE_DECODE_ERROR = 850,
    IMAGE_COPYTO_NOT_SAME_SIZE_ERROR=860,
    IMAGE_TYPE_NOT_SUPPORT=870,
    IMAGE_CROP_SIZE_ERROR=883,
};
