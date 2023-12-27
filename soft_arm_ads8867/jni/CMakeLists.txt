project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/msdk.cpp)
               
set(incs include/msdk.h)

include_directories("include")

add_library(msdk SHARED ${incs}  ${srcs})

target_link_libraries (msdk sdk)

