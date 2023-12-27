project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/sysm.cpp src/sysm_log.cpp
	src/sysm_authorization.cpp	src/sysm_LedAndHeart.cpp)
         
set(incs include/sysm.h include/sysm_log.h
	include/sysm_authorization.h include/sysm_LedAndHeart.h)
         
include_directories("include")

add_library(sysm SHARED ${incs}  ${srcs})

target_link_libraries (sysm  utils sm2 sm2dec aes ql_peripheral ql_common_api mqtt_pub)