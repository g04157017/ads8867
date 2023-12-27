project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/adc_ky.cpp)

set(incs include/adc_ky.h
		 )

include_directories("include")

add_library(adc_ky SHARED ${incs}  ${srcs})
target_link_libraries(adc_ky ql_peripheral ql_common_api cjson iniparser)

