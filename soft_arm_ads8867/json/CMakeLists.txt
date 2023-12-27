project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/cJSON.cpp
         src/CJsonObject.cpp
         src/parson.cpp)

set(incs include/cJSON.h
         include/CJsonObject.h
         include/json.hpp
         include/parson.h)

include_directories("include")

add_library(cjson SHARED ${incs}  ${srcs})