project(${PROJECT_NAME} VERSION 0.1.0)

#set(srcs src/mqtt_pub.cpp)

#set(incs include/mqtt.h)

#include_directories("include")

#add_library(mqtt_pub SHARED ${incs}  ${srcs})
#target_link_libraries(mqtt_pub cjson)
