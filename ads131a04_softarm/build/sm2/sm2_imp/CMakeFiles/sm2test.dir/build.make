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
include sm2/sm2_imp/CMakeFiles/sm2test.dir/depend.make

# Include the progress variables for this target.
include sm2/sm2_imp/CMakeFiles/sm2test.dir/progress.make

# Include the compile flags for this target's objects.
include sm2/sm2_imp/CMakeFiles/sm2test.dir/flags.make

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o: sm2/sm2_imp/CMakeFiles/sm2test.dir/flags.make
sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o: ../sm2/sm2_imp/src/base64.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sm2test.dir/src/base64.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/base64.cpp

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sm2test.dir/src/base64.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/base64.cpp > CMakeFiles/sm2test.dir/src/base64.cpp.i

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sm2test.dir/src/base64.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/base64.cpp -o CMakeFiles/sm2test.dir/src/base64.cpp.s

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.requires:

.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.requires

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.provides: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.requires
	$(MAKE) -f sm2/sm2_imp/CMakeFiles/sm2test.dir/build.make sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.provides.build
.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.provides

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.provides.build: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o


sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o: sm2/sm2_imp/CMakeFiles/sm2test.dir/flags.make
sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o: ../sm2/sm2_imp/src/sm2_Intrfs_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/sm2_Intrfs_test.cpp

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/sm2_Intrfs_test.cpp > CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.i

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/sm2_Intrfs_test.cpp -o CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.s

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.requires:

.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.requires

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.provides: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.requires
	$(MAKE) -f sm2/sm2_imp/CMakeFiles/sm2test.dir/build.make sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.provides.build
.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.provides

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.provides.build: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o


sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o: sm2/sm2_imp/CMakeFiles/sm2test.dir/flags.make
sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o: ../sm2/sm2_imp/src/sm2_libtom.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/sm2_libtom.cpp

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/sm2_libtom.cpp > CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.i

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp/src/sm2_libtom.cpp -o CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.s

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.requires:

.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.requires

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.provides: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.requires
	$(MAKE) -f sm2/sm2_imp/CMakeFiles/sm2test.dir/build.make sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.provides.build
.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.provides

sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.provides.build: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o


# Object files for target sm2test
sm2test_OBJECTS = \
"CMakeFiles/sm2test.dir/src/base64.cpp.o" \
"CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o" \
"CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o"

# External object files for target sm2test
sm2test_EXTERNAL_OBJECTS =

../output/sm2test: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o
../output/sm2test: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o
../output/sm2test: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o
../output/sm2test: sm2/sm2_imp/CMakeFiles/sm2test.dir/build.make
../output/sm2test: ../lib/libsm2.so
../output/sm2test: ../lib/libsm2dec.so
../output/sm2test: ../lib/libtommath.so
../output/sm2test: sm2/sm2_imp/CMakeFiles/sm2test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../../output/sm2test"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sm2test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sm2/sm2_imp/CMakeFiles/sm2test.dir/build: ../output/sm2test

.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/build

sm2/sm2_imp/CMakeFiles/sm2test.dir/requires: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/base64.cpp.o.requires
sm2/sm2_imp/CMakeFiles/sm2test.dir/requires: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_Intrfs_test.cpp.o.requires
sm2/sm2_imp/CMakeFiles/sm2test.dir/requires: sm2/sm2_imp/CMakeFiles/sm2test.dir/src/sm2_libtom.cpp.o.requires

.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/requires

sm2/sm2_imp/CMakeFiles/sm2test.dir/clean:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp && $(CMAKE_COMMAND) -P CMakeFiles/sm2test.dir/cmake_clean.cmake
.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/clean

sm2/sm2_imp/CMakeFiles/sm2test.dir/depend:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ramandrom/sharedir/ads131a04_softarm /home/ramandrom/sharedir/ads131a04_softarm/sm2/sm2_imp /home/ramandrom/sharedir/ads131a04_softarm/build /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp /home/ramandrom/sharedir/ads131a04_softarm/build/sm2/sm2_imp/CMakeFiles/sm2test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sm2/sm2_imp/CMakeFiles/sm2test.dir/depend

