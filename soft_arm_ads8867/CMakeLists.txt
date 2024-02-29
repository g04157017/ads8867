# CMake 最低的版本要求
cmake_minimum_required(VERSION 3.5.1)

# 定义项目名称变量PROJECT_NAME
set(PROJECT_NAME soft_v1)

# 项目名称以及版本
project(${PROJECT_NAME} VERSION 0.1.0)

# 指定通用编译器
#set(CROSS_TOOLCHAIN_PREFIX "/usr/bin") 
#set(CMAKE_C_COMPILER ${CROSS_TOOLCHAIN_PREFIX}/cc)
#set(CMAKE_CXX_COMPILER ${CROSS_TOOLCHAIN_PREFIX}/c++)

# 指定arm编译器
set(CROSS_TOOLCHAIN_PREFIX "/home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi/")
set(CMAKE_C_COMPILER ${CROSS_TOOLCHAIN_PREFIX}/arm-oe-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_TOOLCHAIN_PREFIX}/arm-oe-linux-gnueabi-g++)

# 设置编译选项
set(CMAKE_BUILD_TYPE  "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g -fno-stack-protector")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 设置项目输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

# 项目公共对外头文件目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/pub)

# 项目公共头文件目录
include_directories(${PROJECT_SOURCE_DIR}/json/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/tools/pahomqtt/include)
include_directories(${PROJECT_SOURCE_DIR}/tools/sqlite-autoconf-3360000/include)
include_directories(${PROJECT_SOURCE_DIR}/mqtt/include)
include_directories(${PROJECT_SOURCE_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/database/include)
include_directories(${PROJECT_SOURCE_DIR}/adc/include)
include_directories(${PROJECT_SOURCE_DIR}/ql_oe/include)
include_directories(${PROJECT_SOURCE_DIR}/apps/include)
include_directories(${PROJECT_SOURCE_DIR}/sysm/include)
include_directories(${PROJECT_SOURCE_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/iniparser/include)
include_directories(${PROJECT_SOURCE_DIR}/modbus/include)
include_directories(${PROJECT_SOURCE_DIR}/aes/include)
include_directories(${PROJECT_SOURCE_DIR}/sm2/include)
include_directories(${PROJECT_SOURCE_DIR}/sm2/sm2_imp/include)
include_directories(${PROJECT_SOURCE_DIR}/sm2/sm2_dec/include)
include_directories(${PROJECT_SOURCE_DIR}/cfg/include)
include_directories(${PROJECT_SOURCE_DIR}/iirfilter/include)
include_directories(${PROJECT_SOURCE_DIR}/tools/zlib-1.2.13/include)
include_directories(${PROJECT_SOURCE_DIR}/cgi-bin/include)

# 本地动态连接库的路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/tools/pahomqtt/lib)
link_directories(${PROJECT_SOURCE_DIR}/tools/zlib-1.2.13/lib)
link_directories(${PROJECT_SOURCE_DIR}/tools/sqlite-autoconf-3360000/lib)
link_directories(${PROJECT_SOURCE_DIR}/ql_oe/lib)


# 系统第三方库文件搜索路径
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib  /usr/local/lib NO_DEFAULT_PATH)

# 性能调优定义
#set(PROJ_PERFTOOLS "/home/jscbtools")

#include_directories(${PROJ_PERFTOOLS}/gperftools/include)
#include_directories(${PROJ_PERFTOOLS}/libunwind/include)

#link_directories(${PROJ_PERFTOOLS}/gperftools/lib)
#link_directories(${PROJ_PERFTOOLS}/libunwind/lib)

# 编译平台
add_definitions(-DLINUX)
#add_definitions(-DWIN32)

# 编译开关
#add_definitions(-DARM_FLAG)
#add_definitions(-DVMM_FLAG)
add_definitions(-DDEBUG_FLAG)
add_definitions(-DDEBUG_FOR_FLAG)
#add_definitions(-DPFM_FLAG)
#add_definitions(-DDEBUG)

# 增加所有子模块
add_subdirectory(${PROJECT_SOURCE_DIR}/aes)
add_subdirectory(${PROJECT_SOURCE_DIR}/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/utils)
add_subdirectory(${PROJECT_SOURCE_DIR}/mqtt)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
add_subdirectory(${PROJECT_SOURCE_DIR}/database)
add_subdirectory(${PROJECT_SOURCE_DIR}/adc)
add_subdirectory(${PROJECT_SOURCE_DIR}/apps)
add_subdirectory(${PROJECT_SOURCE_DIR}/sm2)
add_subdirectory(${PROJECT_SOURCE_DIR}/sysm)
add_subdirectory(${PROJECT_SOURCE_DIR}/iniparser)
add_subdirectory(${PROJECT_SOURCE_DIR}/modbus)
add_subdirectory(${PROJECT_SOURCE_DIR}/sgmii)
add_subdirectory(${PROJECT_SOURCE_DIR}/cfg)
add_subdirectory(${PROJECT_SOURCE_DIR}/iirfilter)
add_subdirectory(${PROJECT_SOURCE_DIR}/cgi-bin)
#add_subdirectory(${PROJECT_SOURCE_DIR}/jni)

# 有关第三方库的使用
# openssl
#OPENSSL_FOUND
#set(OPENSSL_ROOT_DIR "/usr/local/openssl")
#set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
#set(OPENSSL_LIBRARIES ${OPENSSL_ROOT_DIR}/lib)
#set(OPENSSL_VERSION 1.0.2g)

# build
# 1、build模式
# mkdir build
# cd build 
# cmake ..
# make
# 2、debug模式，可以进行gdb调试
# mkdir Debug
# cd Debug 
# cmake -DCMAKE_BUILD_TYPE=Debug .. 
# make
# 3、release模式
# mkdir Release
# cd Release 
# cmake -DCMAKE_BUILD_TYPE=Release .. 
# make

