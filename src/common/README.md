
## zip_tool.cpp 使用

在自己的 CMakeLists.txt 增加如下代码
```bash
${COMMON_DIR}/zip_tool.cpp
${COMMON_DIR}/zip/unzip.c
${COMMON_DIR}/zip/zip.c
${COMMON_DIR}/zip/ioapi.c
```

在自己的 CMakeLists.txt 增加如下依赖
```bash
IF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
SET(ZLIB_PATH "D:\\work\\lib\\zlib-1.2.11\\install_2017_mt")
set(ZLIB_LIB
    ${ZLIB_PATH}/lib/zlibstat.lib
)
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
SET(ZLIB_PATH "${PREFIX}/work/lib/zlib-1.2.13/install.${CUR_ARCH}")
set(ZLIB_LIB
    ${ZLIB_PATH}/lib/libz.a
)
ENDIF()
include_directories(${ZLIB_PATH}/include)
target_link_libraries(${TARGET_NAME} ${ZLIB_LIB})
```


## serial_tool.cpp 使用
