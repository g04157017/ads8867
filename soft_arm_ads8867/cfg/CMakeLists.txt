project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/alarm.cpp
		 src/otaload.cpp
)

set(incs include/alarm.h
		 include/otaload.h
)

include_directories("include")

add_library(cfg SHARED ${incs}  ${srcs})
target_link_libraries(cfg cjson iniparser mqtt_pub)
