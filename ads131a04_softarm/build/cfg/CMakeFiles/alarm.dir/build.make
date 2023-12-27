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
CMAKE_SOURCE_DIR = /home/ramandrom/sharedir/ads131a04_softarm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ramandrom/sharedir/ads131a04_softarm/build

# Include any dependencies generated for this target.
include cfg/CMakeFiles/alarm.dir/depend.make

# Include the progress variables for this target.
include cfg/CMakeFiles/alarm.dir/progress.make

# Include the compile flags for this target's objects.
include cfg/CMakeFiles/alarm.dir/flags.make

cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o: cfg/CMakeFiles/alarm.dir/flags.make
cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o: ../cfg/src/alarm.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/cfg && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alarm.dir/src/alarm.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/cfg/src/alarm.cpp

cfg/CMakeFiles/alarm.dir/src/alarm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alarm.dir/src/alarm.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/cfg && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/cfg/src/alarm.cpp > CMakeFiles/alarm.dir/src/alarm.cpp.i

cfg/CMakeFiles/alarm.dir/src/alarm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alarm.dir/src/alarm.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/cfg && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/cfg/src/alarm.cpp -o CMakeFiles/alarm.dir/src/alarm.cpp.s

cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.requires:

.PHONY : cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.requires

cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.provides: cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.requires
	$(MAKE) -f cfg/CMakeFiles/alarm.dir/build.make cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.provides.build
.PHONY : cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.provides

cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.provides.build: cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o


# Object files for target alarm
alarm_OBJECTS = \
"CMakeFiles/alarm.dir/src/alarm.cpp.o"

# External object files for target alarm
alarm_EXTERNAL_OBJECTS =

../lib/libalarm.so: cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o
../lib/libalarm.so: cfg/CMakeFiles/alarm.dir/build.make
../lib/libalarm.so: ../lib/libmqtt_pub.so
../lib/libalarm.so: ../lib/libcjson.so
../lib/libalarm.so: ../lib/libiniparser.so
../lib/libalarm.so: ../lib/libkydatabase.so
../lib/libalarm.so: cfg/CMakeFiles/alarm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libalarm.so"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/cfg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/alarm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
cfg/CMakeFiles/alarm.dir/build: ../lib/libalarm.so

.PHONY : cfg/CMakeFiles/alarm.dir/build

cfg/CMakeFiles/alarm.dir/requires: cfg/CMakeFiles/alarm.dir/src/alarm.cpp.o.requires

.PHONY : cfg/CMakeFiles/alarm.dir/requires

cfg/CMakeFiles/alarm.dir/clean:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/cfg && $(CMAKE_COMMAND) -P CMakeFiles/alarm.dir/cmake_clean.cmake
.PHONY : cfg/CMakeFiles/alarm.dir/clean

cfg/CMakeFiles/alarm.dir/depend:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ramandrom/sharedir/ads131a04_softarm /home/ramandrom/sharedir/ads131a04_softarm/cfg /home/ramandrom/sharedir/ads131a04_softarm/build /home/ramandrom/sharedir/ads131a04_softarm/build/cfg /home/ramandrom/sharedir/ads131a04_softarm/build/cfg/CMakeFiles/alarm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cfg/CMakeFiles/alarm.dir/depend
