project(${PROJECT_NAME} VERSION 0.1.0)

set(srcs src/main_test.cpp)

set(src_pub_client src/pub_client_test.cpp)

set(src_adc src/example_adc.c)
set(src_spi src/example_spi.c)

set(src_database src/database_test.cpp)

include_directories("include")

add_executable(test_main ${srcs})

add_executable(pub_client_test ${src_pub_client})
target_link_libraries(pub_client_test mqtt_pub paho-mqtt3c)

add_executable(database_test ${src_database})
target_link_libraries(database_test  kydatabase  sqlite3)

add_executable(adc_test ${src_adc})
target_link_libraries(adc_test ql_peripheral)

add_executable(spi_test ${src_spi})
#target_link_libraries(spi_test pthread)
target_link_libraries(spi_test ql_peripheral ql_common_api)


set(src_modbus src/modbus_test.cpp)
add_executable(modbus_test ${src_modbus})
target_link_libraries(modbus_test modbus pthread ql_peripheral ql_common_api)