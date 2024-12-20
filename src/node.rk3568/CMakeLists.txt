SET(CUR_ARCH "aarch64")
# SET(CUR_ARCH "armhf")
# SET(CUR_ARCH "x86_64")
# SET(PREFIX /home/sunjie/work)
SET(PREFIX /home/niu/work)

SET(CMAKE_C_FLAGS "-O3 -fPIC -ldl -lpthread -lrt")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++14 -fpermissive -DWITH_OPENSSL -DWITH_DOM -DWITH_NONAMESPACES -Wl,-unresolved-symbols=ignore-in-shared-libs")

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

SET(PROJECT_DIR ${PROJECT_SOURCE_DIR})
SET(UTIL_DIR ${PROJECT_DIR}/base_util)
SET(COMMON_DIR ${PROJECT_DIR}/common)
SET(NODEBASE_DIR ${PROJECT_DIR}/node.base)
SET(NODE_DIR ${PROJECT_DIR}/node.rk3568)
aux_source_directory(${UTIL_DIR} util_src)
aux_source_directory(${NODEBASE_DIR} nodebase_src)
aux_source_directory(${NODE_DIR} node_src)

SET(BOTH_SOURCE
    ${util_src}
    ${COMMON_DIR}/zip_tool.cpp
    ${COMMON_DIR}/zip/unzip.c
    ${COMMON_DIR}/zip/zip.c
    ${COMMON_DIR}/zip/ioapi.c
    
    # ${COMMON_DIR}/http_tool.cpp
    # ${COMMON_DIR}/mediakit_tool.cpp
    ${COMMON_DIR}/common_tool.cpp
    ${COMMON_DIR}/mqtt_tool.cpp
)

SET(NODE_SOURCE
    ${BOTH_SOURCE}

    # ${AUTH_DIR}/aes_128_ecb/aes_ecb.cpp
    # ${AUTH_DIR}/src/base_tools.cpp
    # ${AUTH_DIR}/src/auth.cpp

    # ${AIMODEL_DIR}/common/processor.cpp
    # ${AIMODEL_DIR}/common/transforms.cpp
    # ${AIMODEL_DIR}/common/visualize.cpp

    ${nodebase_src}
    ${node_src}
)


SET(EXEC_NAME oil_node)
add_executable(${EXEC_NAME} ${NODE_SOURCE})

include_directories(${PROJECT_DIR})
# include_directories(${AUTH_DIR})
# ffmpeg
SET(FFMPEG_PATH ${PREFIX}/lib/ffmpeg_build.${CUR_ARCH})
set(FFMPEG_LIB
    ${FFMPEG_PATH}/lib/libavcodec.so.58
    ${FFMPEG_PATH}/lib/libavformat.so.58
    ${FFMPEG_PATH}/lib/libavutil.so.56
    ${FFMPEG_PATH}/lib/libswresample.so.3
    ${FFMPEG_PATH}/lib/libx264.so.164
)
include_directories(${FFMPEG_PATH}/include)
target_link_libraries(${EXEC_NAME} ${FFMPEG_LIB})
# freetype
SET(FREE_PATH ${PREFIX}/lib/freetype-2.13.2/install.${CUR_ARCH})
set(FREE_LIB ${FREE_PATH}/lib/libfreetype.so.6.20.1)
include_directories(${FREE_PATH}/include/freetype2)
target_link_libraries(${EXEC_NAME} ${FREE_LIB})

#harfbuzz
SET(HARF_PATH ${PREFIX}/lib/harfbuzz-2.7.2/install.${CUR_ARCH}.ft)
set(HARF_LIB ${HARF_PATH}/lib/libharfbuzz.so.0.20702.0)
include_directories(${HARF_PATH}/include/harfbuzz)
target_link_libraries(${EXEC_NAME} ${HARF_LIB} ${HARF_LIB})

# opencv3.4.5
SET(OPENCV_PATH ${PREFIX}/lib/opencv-3.4.5/install.${CUR_ARCH}.freetype)
set(OPENCV_LIB ${OPENCV_PATH}/lib/libopencv_world.so.3.4)
include_directories(${OPENCV_PATH}/include)
target_link_libraries(${EXEC_NAME} ${OPENCV_LIB})


# openssl
SET(OPENSSL_PATH "${PREFIX}/lib/openssl-1.1.1q/install.${CUR_ARCH}")
set(OPENSSL_LIB
    ${OPENSSL_PATH}/lib/libcrypto.so.1.1
    ${OPENSSL_PATH}/lib/libssl.so.1.1
)
include_directories(${OPENSSL_PATH}/include)
target_link_libraries(${EXEC_NAME} ${OPENSSL_LIB})

# # osip2
# SET(OSIP2_PATH "${PREFIX}/lib/libosip2-5.2.0/install.${CUR_ARCH}")
# set(OSIP2_LIB
#     ${OSIP2_PATH}/lib/libosip2.so
#     ${OSIP2_PATH}/lib/libosipparser2.so
# )
# include_directories(${OSIP2_PATH}/include)
# target_link_libraries(${EXEC_NAME} ${OSIP2_LIB})

# # exosip2
# SET(EXOSIP2_PATH "${PREFIX}/lib/libexosip2-5.2.0/install.${CUR_ARCH}")
# set(EXOSIP2_LIB
#     ${EXOSIP2_PATH}/lib/libeXosip2.so
# )
# include_directories(${EXOSIP2_PATH}/include)
# target_link_libraries(${EXEC_NAME} ${EXOSIP2_LIB})

#libiconv
SET(ICONV_PATH "${PREFIX}/lib/libiconv-1.15/install.${CUR_ARCH}")
set(ICONV_LIB
    ${ICONV_PATH}/lib/libiconv.so.2
    ${ICONV_PATH}/lib/libcharset.so.1
)
include_directories(${ICONV_PATH}/include)
target_link_libraries(${EXEC_NAME} ${ICONV_LIB})

# mqtt
SET(MQTT_PATH "${PREFIX}/lib/mosquitto-2.0.18/install.${CUR_ARCH}")
set(MQTT_LIB
    ${MQTT_PATH}/lib/libmosquittopp.so.1
    ${MQTT_PATH}/lib/libmosquitto.so.1
)
include_directories(${MQTT_PATH}/include)
target_link_libraries(${EXEC_NAME} ${MQTT_LIB})

# zlib
SET(ZLIB_PATH "${PREFIX}/lib/zlib-1.2.13/install.${CUR_ARCH}")
set(ZLIB_LIB
    ${ZLIB_PATH}/lib/libz.a
)
include_directories(${ZLIB_PATH}/include)
target_link_libraries(${EXEC_NAME} ${ZLIB_LIB})
# modbus
SET(MODBUS_PATH /home/niu/work/lib/libmodbus-3.0.8/install.${CUR_ARCH})
set(MODBUS_LIB ${MODBUS_PATH}/lib/libmodbus.so)
include_directories(${MODBUS_PATH}/include)
target_link_libraries(${EXEC_NAME} ${MODBUS_LIB})


# boost
SET(BOOST_PATH "${PREFIX}/lib/boost_1_67_0")
set(BOOST_LIB
    ${BOOST_PATH}/stage/lib.${CUR_ARCH}/libboost_filesystem.a
    ${BOOST_PATH}/stage/lib.${CUR_ARCH}/libboost_system.a
    ${BOOST_PATH}/stage/lib.${CUR_ARCH}/libboost_date_time.a
    ${BOOST_PATH}/stage/lib.${CUR_ARCH}/libboost_regex.a
)
include_directories(${BOOST_PATH})
target_link_libraries(${EXEC_NAME} ${BOOST_LIB})


# json
SET(JSONCPP_PATH ${PREFIX}/lib/jsoncpp-1.9.5/install.${CUR_ARCH})
set(JSONCPP_LIB ${JSONCPP_PATH}/lib/libjsoncpp.a)
include_directories(${JSONCPP_PATH}/include)
target_link_libraries(${EXEC_NAME} ${JSONCPP_LIB})

# spdlog
SET(SPDLOG_PATH ${PREFIX}/lib/spdlog-1.9.2/install.${CUR_ARCH})
set(SPDLOG_LIB ${SPDLOG_PATH}/lib/libspdlog.a)
include_directories(${SPDLOG_PATH}/include)
target_link_libraries(${EXEC_NAME} ${SPDLOG_LIB})
# # modbus
# SET(MODBUS_PATH /home/niu/work/lib/libmodbus-3.0.8/install.${CUR_ARCH})
# set(MODBUS_LIB ${MODBUS_PATH}/lib/libmodbus.so)
# include_directories(${MODBUS_PATH}/include)
# target_link_libraries(${TARGET_NAME} ${MODBUS_LIB})
# # yaml
# SET(YAML_PATH ${PREFIX}/lib/yaml-cpp-yaml-cpp-0.6.2/install.${CUR_ARCH})
# set(YAML_LIB ${YAML_PATH}/lib/libyaml-cpp.a)
# include_directories(${YAML_PATH}/include)
# target_link_libraries(${EXEC_NAME} ${YAML_LIB})


target_link_libraries(${EXEC_NAME} ${CMAKE_CXX_FLAGS})

