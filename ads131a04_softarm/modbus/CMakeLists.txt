project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/modbus.c
		 src/modbus-data.c
		 src/modbus-rtu.c
		 src/modbus-tcp.c
)

set(incs include/modbus_config.h
		 include/modbus.h
		 include/modbus-private.h
		 include/modbus-rtu.h
		 include/modbus-rtu-private.h
		 include/modbus-tcp.h
		 include/modbus-tcp-private.h
		 include/modbus-version.h
)

include_directories("include")

add_library(modbus SHARED ${incs}  ${srcs})
target_link_libraries(modbus ql_peripheral ql_common_api)


