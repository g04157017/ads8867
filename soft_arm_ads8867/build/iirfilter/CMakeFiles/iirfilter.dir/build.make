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
include iirfilter/CMakeFiles/iirfilter.dir/depend.make

# Include the progress variables for this target.
include iirfilter/CMakeFiles/iirfilter.dir/progress.make

# Include the compile flags for this target's objects.
include iirfilter/CMakeFiles/iirfilter.dir/flags.make

iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o: iirfilter/CMakeFiles/iirfilter.dir/flags.make
iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o: ../iirfilter/src/iirfilter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/soft_arm_ads8867/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o -c /home/ramandrom/sharedir/soft_arm_ads8867/iirfilter/src/iirfilter.cpp

iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/iirfilter.dir/src/iirfilter.cpp.i"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/soft_arm_ads8867/iirfilter/src/iirfilter.cpp > CMakeFiles/iirfilter.dir/src/iirfilter.cpp.i

iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/iirfilter.dir/src/iirfilter.cpp.s"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/soft_arm_ads8867/iirfilter/src/iirfilter.cpp -o CMakeFiles/iirfilter.dir/src/iirfilter.cpp.s

iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.requires:

.PHONY : iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.requires

iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.provides: iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.requires
	$(MAKE) -f iirfilter/CMakeFiles/iirfilter.dir/build.make iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.provides.build
.PHONY : iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.provides

iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.provides.build: iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o


# Object files for target iirfilter
iirfilter_OBJECTS = \
"CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o"

# External object files for target iirfilter
iirfilter_EXTERNAL_OBJECTS =

../lib/libiirfilter.so: iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o
../lib/libiirfilter.so: iirfilter/CMakeFiles/iirfilter.dir/build.make
../lib/libiirfilter.so: iirfilter/CMakeFiles/iirfilter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ramandrom/sharedir/soft_arm_ads8867/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libiirfilter.so"
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/iirfilter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
iirfilter/CMakeFiles/iirfilter.dir/build: ../lib/libiirfilter.so

.PHONY : iirfilter/CMakeFiles/iirfilter.dir/build

iirfilter/CMakeFiles/iirfilter.dir/requires: iirfilter/CMakeFiles/iirfilter.dir/src/iirfilter.cpp.o.requires

.PHONY : iirfilter/CMakeFiles/iirfilter.dir/requires

iirfilter/CMakeFiles/iirfilter.dir/clean:
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter && $(CMAKE_COMMAND) -P CMakeFiles/iirfilter.dir/cmake_clean.cmake
.PHONY : iirfilter/CMakeFiles/iirfilter.dir/clean

iirfilter/CMakeFiles/iirfilter.dir/depend:
	cd /home/ramandrom/sharedir/soft_arm_ads8867/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ramandrom/sharedir/soft_arm_ads8867 /home/ramandrom/sharedir/soft_arm_ads8867/iirfilter /home/ramandrom/sharedir/soft_arm_ads8867/build /home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter /home/ramandrom/sharedir/soft_arm_ads8867/build/iirfilter/CMakeFiles/iirfilter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : iirfilter/CMakeFiles/iirfilter.dir/depend

