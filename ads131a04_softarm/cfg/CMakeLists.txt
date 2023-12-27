project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/alarm.cpp)

set(incs include/alarm.h)

include_directories("include")

add_library(alarm SHARED ${incs}  ${srcs})
target_link_libraries(alarm cjson iniparser mqtt_pub)
