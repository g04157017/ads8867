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
include test/CMakeFiles/pub_client_test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/pub_client_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/pub_client_test.dir/flags.make

test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o: test/CMakeFiles/pub_client_test.dir/flags.make
test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o: ../test/src/pub_client_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/test && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/test/src/pub_client_test.cpp

test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/test && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/test/src/pub_client_test.cpp > CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.i

test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/test && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/test/src/pub_client_test.cpp -o CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.s

test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.requires:

.PHONY : test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.requires

test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.provides: test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/pub_client_test.dir/build.make test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.provides.build
.PHONY : test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.provides

test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.provides.build: test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o


# Object files for target pub_client_test
pub_client_test_OBJECTS = \
"CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o"

# External object files for target pub_client_test
pub_client_test_EXTERNAL_OBJECTS =

../output/pub_client_test: test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o
../output/pub_client_test: test/CMakeFiles/pub_client_test.dir/build.make
../output/pub_client_test: ../lib/libmqtt_pub.so
../output/pub_client_test: ../lib/libcjson.so
../output/pub_client_test: ../lib/libiniparser.so
../output/pub_client_test: ../lib/libkydatabase.so
../output/pub_client_test: test/CMakeFiles/pub_client_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../output/pub_client_test"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pub_client_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/pub_client_test.dir/build: ../output/pub_client_test

.PHONY : test/CMakeFiles/pub_client_test.dir/build

test/CMakeFiles/pub_client_test.dir/requires: test/CMakeFiles/pub_client_test.dir/src/pub_client_test.cpp.o.requires

.PHONY : test/CMakeFiles/pub_client_test.dir/requires

test/CMakeFiles/pub_client_test.dir/clean:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/test && $(CMAKE_COMMAND) -P CMakeFiles/pub_client_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/pub_client_test.dir/clean

test/CMakeFiles/pub_client_test.dir/depend:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ramandrom/sharedir/ads131a04_softarm /home/ramandrom/sharedir/ads131a04_softarm/test /home/ramandrom/sharedir/ads131a04_softarm/build /home/ramandrom/sharedir/ads131a04_softarm/build/test /home/ramandrom/sharedir/ads131a04_softarm/build/test/CMakeFiles/pub_client_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/pub_client_test.dir/depend

