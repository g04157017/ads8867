project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/cgi_main.cpp)

set(incs include/cgi_main.h
		 )

include_directories("include")

add_executable(cgi_test ${incs}  ${srcs})
#target_link_libraries(adc_ky ql_peripheral ql_common_api cjson iniparser)

