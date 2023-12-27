project(${PROJECT_NAME} VERSION 0.1.0)

aux_source_directory(. DIR_SRCS)

add_library(tommath SHARED ${DIR_SRCS})
