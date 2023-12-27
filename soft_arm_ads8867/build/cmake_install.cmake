# Install script for directory: /home/ramandrom/sharedir/soft_arm_ads8867

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/aes/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/utils/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/json/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/test/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/database/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/adc/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/apps/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/sm2/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/sysm/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/iniparser/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/modbus/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/sgmii/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/cfg/cmake_install.cmake")
  include("/home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ramandrom/sharedir/soft_arm_ads8867/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")