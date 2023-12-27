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
include adc/CMakeFiles/adc_ky.dir/depend.make

# Include the progress variables for this target.
include adc/CMakeFiles/adc_ky.dir/progress.make

# Include the compile flags for this target's objects.
include adc/CMakeFiles/adc_ky.dir/flags.make

adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o: adc/CMakeFiles/adc_ky.dir/flags.make
adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o: ../adc/src/adc_ky.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/soft_arm_ads8867/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/adc && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o -c /home/ramandrom/sharedir/soft_arm_ads8867/adc/src/adc_ky.cpp

adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adc_ky.dir/src/adc_ky.cpp.i"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/adc && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/soft_arm_ads8867/adc/src/adc_ky.cpp > CMakeFiles/adc_ky.dir/src/adc_ky.cpp.i

adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adc_ky.dir/src/adc_ky.cpp.s"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/adc && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/soft_arm_ads8867/adc/src/adc_ky.cpp -o CMakeFiles/adc_ky.dir/src/adc_ky.cpp.s

adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.requires:

.PHONY : adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.requires

adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.provides: adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.requires
	$(MAKE) -f adc/CMakeFiles/adc_ky.dir/build.make adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.provides.build
.PHONY : adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.provides

adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.provides.build: adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o


# Object files for target adc_ky
adc_ky_OBJECTS = \
"CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o"

# External object files for target adc_ky
adc_ky_EXTERNAL_OBJECTS =

../lib/libadc_ky.so: adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o
../lib/libadc_ky.so: adc/CMakeFiles/adc_ky.dir/build.make
../lib/libadc_ky.so: ../lib/libcjson.so
../lib/libadc_ky.so: ../lib/libiniparser.so
../lib/libadc_ky.so: adc/CMakeFiles/adc_ky.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ramandrom/sharedir/soft_arm_ads8867/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libadc_ky.so"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/adc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/adc_ky.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
adc/CMakeFiles/adc_ky.dir/build: ../lib/libadc_ky.so

.PHONY : adc/CMakeFiles/adc_ky.dir/build

adc/CMakeFiles/adc_ky.dir/requires: adc/CMakeFiles/adc_ky.dir/src/adc_ky.cpp.o.requires

.PHONY : adc/CMakeFiles/adc_ky.dir/requires

adc/CMakeFiles/adc_ky.dir/clean:
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/adc && $(CMAKE_COMMAND) -P CMakeFiles/adc_ky.dir/cmake_clean.cmake
.PHONY : adc/CMakeFiles/adc_ky.dir/clean

adc/CMakeFiles/adc_ky.dir/depend:
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ramandrom/sharedir/soft_arm_ads8867 /home/ramandrom/sharedir/soft_arm_ads8867/adc /home/ramandrom/sharedir/soft_arm_ads8867/build /home/ramandrom/sharedir/soft_arm_ads8867/build/adc /home/ramandrom/sharedir/soft_arm_ads8867/build/adc/CMakeFiles/adc_ky.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : adc/CMakeFiles/adc_ky.dir/depend

