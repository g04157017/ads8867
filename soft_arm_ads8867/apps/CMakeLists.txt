project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/main.cpp
		 src/app.cpp)
 
set(incs include/app.h)

include_directories("include")

add_executable(app_ex ${srcs}   ${incs})
target_link_libraries(app_ex adc_ky kydatabase mqtt_pub sysm iniparser ql_peripheral ql_common_api cfg)

