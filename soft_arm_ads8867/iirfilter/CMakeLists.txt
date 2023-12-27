project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/iirfilter.cpp)

set(incs include/iirfilter.h
		 )

include_directories("include")

add_library(iirfilter SHARED ${incs}  ${srcs})
#target_link_libraries(adc_ky ql_peripheral ql_common_api cjson iniparser)

