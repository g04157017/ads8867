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
include json/CMakeFiles/cjson.dir/depend.make

# Include the progress variables for this target.
include json/CMakeFiles/cjson.dir/progress.make

# Include the compile flags for this target's objects.
include json/CMakeFiles/cjson.dir/flags.make

json/CMakeFiles/cjson.dir/src/cJSON.cpp.o: json/CMakeFiles/cjson.dir/flags.make
json/CMakeFiles/cjson.dir/src/cJSON.cpp.o: ../json/src/cJSON.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object json/CMakeFiles/cjson.dir/src/cJSON.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cjson.dir/src/cJSON.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/json/src/cJSON.cpp

json/CMakeFiles/cjson.dir/src/cJSON.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cjson.dir/src/cJSON.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/json/src/cJSON.cpp > CMakeFiles/cjson.dir/src/cJSON.cpp.i

json/CMakeFiles/cjson.dir/src/cJSON.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cjson.dir/src/cJSON.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/json/src/cJSON.cpp -o CMakeFiles/cjson.dir/src/cJSON.cpp.s

json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.requires:

.PHONY : json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.requires

json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.provides: json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.requires
	$(MAKE) -f json/CMakeFiles/cjson.dir/build.make json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.provides.build
.PHONY : json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.provides

json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.provides.build: json/CMakeFiles/cjson.dir/src/cJSON.cpp.o


json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o: json/CMakeFiles/cjson.dir/flags.make
json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o: ../json/src/CJsonObject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cjson.dir/src/CJsonObject.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/json/src/CJsonObject.cpp

json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cjson.dir/src/CJsonObject.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/json/src/CJsonObject.cpp > CMakeFiles/cjson.dir/src/CJsonObject.cpp.i

json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cjson.dir/src/CJsonObject.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/json/src/CJsonObject.cpp -o CMakeFiles/cjson.dir/src/CJsonObject.cpp.s

json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.requires:

.PHONY : json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.requires

json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.provides: json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.requires
	$(MAKE) -f json/CMakeFiles/cjson.dir/build.make json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.provides.build
.PHONY : json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.provides

json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.provides.build: json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o


json/CMakeFiles/cjson.dir/src/parson.cpp.o: json/CMakeFiles/cjson.dir/flags.make
json/CMakeFiles/cjson.dir/src/parson.cpp.o: ../json/src/parson.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object json/CMakeFiles/cjson.dir/src/parson.cpp.o"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cjson.dir/src/parson.cpp.o -c /home/ramandrom/sharedir/ads131a04_softarm/json/src/parson.cpp

json/CMakeFiles/cjson.dir/src/parson.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cjson.dir/src/parson.cpp.i"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ramandrom/sharedir/ads131a04_softarm/json/src/parson.cpp > CMakeFiles/cjson.dir/src/parson.cpp.i

json/CMakeFiles/cjson.dir/src/parson.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cjson.dir/src/parson.cpp.s"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-crosstool/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi//arm-oe-linux-gnueabi-g++   -march=armv7-a -mfloat-abi=softfp -mfpu=neon $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ramandrom/sharedir/ads131a04_softarm/json/src/parson.cpp -o CMakeFiles/cjson.dir/src/parson.cpp.s

json/CMakeFiles/cjson.dir/src/parson.cpp.o.requires:

.PHONY : json/CMakeFiles/cjson.dir/src/parson.cpp.o.requires

json/CMakeFiles/cjson.dir/src/parson.cpp.o.provides: json/CMakeFiles/cjson.dir/src/parson.cpp.o.requires
	$(MAKE) -f json/CMakeFiles/cjson.dir/build.make json/CMakeFiles/cjson.dir/src/parson.cpp.o.provides.build
.PHONY : json/CMakeFiles/cjson.dir/src/parson.cpp.o.provides

json/CMakeFiles/cjson.dir/src/parson.cpp.o.provides.build: json/CMakeFiles/cjson.dir/src/parson.cpp.o


# Object files for target cjson
cjson_OBJECTS = \
"CMakeFiles/cjson.dir/src/cJSON.cpp.o" \
"CMakeFiles/cjson.dir/src/CJsonObject.cpp.o" \
"CMakeFiles/cjson.dir/src/parson.cpp.o"

# External object files for target cjson
cjson_EXTERNAL_OBJECTS =

../lib/libcjson.so: json/CMakeFiles/cjson.dir/src/cJSON.cpp.o
../lib/libcjson.so: json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o
../lib/libcjson.so: json/CMakeFiles/cjson.dir/src/parson.cpp.o
../lib/libcjson.so: json/CMakeFiles/cjson.dir/build.make
../lib/libcjson.so: json/CMakeFiles/cjson.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ramandrom/sharedir/ads131a04_softarm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ../../lib/libcjson.so"
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cjson.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
json/CMakeFiles/cjson.dir/build: ../lib/libcjson.so

.PHONY : json/CMakeFiles/cjson.dir/build

json/CMakeFiles/cjson.dir/requires: json/CMakeFiles/cjson.dir/src/cJSON.cpp.o.requires
json/CMakeFiles/cjson.dir/requires: json/CMakeFiles/cjson.dir/src/CJsonObject.cpp.o.requires
json/CMakeFiles/cjson.dir/requires: json/CMakeFiles/cjson.dir/src/parson.cpp.o.requires

.PHONY : json/CMakeFiles/cjson.dir/requires

json/CMakeFiles/cjson.dir/clean:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build/json && $(CMAKE_COMMAND) -P CMakeFiles/cjson.dir/cmake_clean.cmake
.PHONY : json/CMakeFiles/cjson.dir/clean

json/CMakeFiles/cjson.dir/depend:
	cd /home/ramandrom/sharedir/ads131a04_softarm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ramandrom/sharedir/ads131a04_softarm /home/ramandrom/sharedir/ads131a04_softarm/json /home/ramandrom/sharedir/ads131a04_softarm/build /home/ramandrom/sharedir/ads131a04_softarm/build/json /home/ramandrom/sharedir/ads131a04_softarm/build/json/CMakeFiles/cjson.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : json/CMakeFiles/cjson.dir/depend

