# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ramandrom/sharedir/soft_arm_ads8867

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ramandrom/sharedir/soft_arm_ads8867/build

# Include any dependencies generated for this target.
include mqtt/CMakeFiles/mqtt_pub.dir/depend.make

# Include the progress variables for this target.
include mqtt/CMakeFiles/mqtt_pub.dir/progress.make

# Include the compile flags for this target's objects.
include mqtt/CMakeFiles/mqtt_pub.dir/flags.make

mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o: mqtt/CMakeFiles/mqtt_pub.dir/flags.make
mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o: ../mqtt/src/mqtt_pub.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/soft_arm_ads8867/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o -c /home/ramandrom/sharedir/soft_arm_ads8867/mqtt/src/mqtt_pub.cpp

mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.i"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/soft_arm_ads8867/mqtt/src/mqtt_pub.cpp > CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.i

mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.s"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/soft_arm_ads8867/mqtt/src/mqtt_pub.cpp -o CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.s

mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.requires:

.PHONY : mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.requires

mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.provides: mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.requires
	$(MAKE) -f mqtt/CMakeFiles/mqtt_pub.dir/build.make mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.provides.build
.PHONY : mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.provides

mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.provides.build: mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o


# Object files for target mqtt_pub
mqtt_pub_OBJECTS = \
"CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o"

# External object files for target mqtt_pub
mqtt_pub_EXTERNAL_OBJECTS =

../lib/libmqtt_pub.so: mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o
../lib/libmqtt_pub.so: mqtt/CMakeFiles/mqtt_pub.dir/build.make
../lib/libmqtt_pub.so: ../lib/libcjson.so
../lib/libmqtt_pub.so: ../lib/libiniparser.so
../lib/libmqtt_pub.so: ../lib/libkydatabase.so
../lib/libmqtt_pub.so: mqtt/CMakeFiles/mqtt_pub.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ramandrom/sharedir/soft_arm_ads8867/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libmqtt_pub.so"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mqtt_pub.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mqtt/CMakeFiles/mqtt_pub.dir/build: ../lib/libmqtt_pub.so

.PHONY : mqtt/CMakeFiles/mqtt_pub.dir/build

mqtt/CMakeFiles/mqtt_pub.dir/requires: mqtt/CMakeFiles/mqtt_pub.dir/src/mqtt_pub.cpp.o.requires

.PHONY : mqtt/CMakeFiles/mqtt_pub.dir/requires

mqtt/CMakeFiles/mqtt_pub.dir/clean:
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt && $(CMAKE_COMMAND) -P CMakeFiles/mqtt_pub.dir/cmake_clean.cmake
.PHONY : mqtt/CMakeFiles/mqtt_pub.dir/clean

mqtt/CMakeFiles/mqtt_pub.dir/depend:
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ramandrom/sharedir/soft_arm_ads8867 /home/ramandrom/sharedir/soft_arm_ads8867/mqtt /home/ramandrom/sharedir/soft_arm_ads8867/build /home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt /home/ramandrom/sharedir/soft_arm_ads8867/build/mqtt/CMakeFiles/mqtt_pub.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mqtt/CMakeFiles/mqtt_pub.dir/depend

