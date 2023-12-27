project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/utils.cpp)
            
set(incs include/utils.h)

include_directories("include")

add_library(utils SHARED ${incs}  ${srcs})

target_link_libraries(utils cjson)


