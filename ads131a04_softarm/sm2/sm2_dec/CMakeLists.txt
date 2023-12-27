project(${PROJECT_NAME} VERSION 0.1.0)

aux_source_directory(. DIR_SRCS)

include_directories("include")

add_library(sm2dec SHARED ${DIR_SRCS})

target_link_libraries (sm2dec)
