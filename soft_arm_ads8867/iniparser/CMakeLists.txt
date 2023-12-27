project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/iniparser.cpp
         src/dictionary.cpp
         src/iniext.cpp)
         
set(incs include/iniparser.h
         include/dictionary.h
         include/iniext.h)
         
include_directories("include")

add_library(iniparser SHARED ${incs}  ${srcs})

