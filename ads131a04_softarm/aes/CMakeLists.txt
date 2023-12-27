project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/keyExtend.cpp
		 src/encryption.cpp
		 src/decrypt.cpp
		 src/main.cpp
)

set(incs include/keyExtend.h
		 include/encryption.h
		 include/decrypt.h
)

include_directories("include")


add_executable(aestest  ${incs} ${srcs} )

add_library(aes SHARED ${incs}  ${srcs})
