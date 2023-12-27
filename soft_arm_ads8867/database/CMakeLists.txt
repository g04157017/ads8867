project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/database.cpp)

set(incs include/kydatabase.h)

include_directories("include")

add_library(kydatabase SHARED ${incs}  ${srcs})
target_link_libraries(kydatabase sqlite3)
