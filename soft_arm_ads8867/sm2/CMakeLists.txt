project(${PROJECT_NAME} VERSION 0.1.0)

# 增加所有子模块
add_subdirectory(${PROJECT_SOURCE_DIR}/libtommath)
add_subdirectory(${PROJECT_SOURCE_DIR}/sm2_imp)
add_subdirectory(${PROJECT_SOURCE_DIR}/sm2_dec)