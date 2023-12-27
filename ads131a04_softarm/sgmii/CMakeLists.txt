project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/sgmii.cpp)
 
#set(incs include/app.h)

include_directories("include")

add_executable(sgmiii_ex ${srcs}   ${incs})
target_link_libraries(sgmiii_ex ql_common_api)

