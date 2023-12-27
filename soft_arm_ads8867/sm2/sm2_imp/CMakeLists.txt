project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs 	 src/sm2.cpp
			 src/base64.cpp
             src/sm3.cpp)

set(tests 	 src/base64.cpp
             src/sm2_Intrfs_test.cpp
             src/sm2_libtom.cpp)

include_directories("include")

set(incs include/GM_define.h
            include/sm2.h
            include/sm3.h
            include/base64.h)

add_library(sm2 SHARED ${incs}  ${srcs})
target_link_libraries (sm2 tommath)

add_executable(sm2test ${incs} ${tests})
target_link_libraries (sm2test sm2 sm2dec)
