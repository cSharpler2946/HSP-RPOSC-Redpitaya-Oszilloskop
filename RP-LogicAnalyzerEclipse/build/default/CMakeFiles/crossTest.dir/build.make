# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/michael/eclipse-workspace/RP-LogicAnalyzer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/michael/eclipse-workspace/RP-LogicAnalyzer/build/default

# Include any dependencies generated for this target.
include CMakeFiles/crossTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/crossTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/crossTest.dir/flags.make

CMakeFiles/crossTest.dir/src/main.cpp.o: CMakeFiles/crossTest.dir/flags.make
CMakeFiles/crossTest.dir/src/main.cpp.o: ../../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/eclipse-workspace/RP-LogicAnalyzer/build/default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/crossTest.dir/src/main.cpp.o"
	/home/michael/eclipse-workspace/RP-LogicAnalyzer/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crossTest.dir/src/main.cpp.o -c /home/michael/eclipse-workspace/RP-LogicAnalyzer/src/main.cpp

CMakeFiles/crossTest.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crossTest.dir/src/main.cpp.i"
	/home/michael/eclipse-workspace/RP-LogicAnalyzer/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/eclipse-workspace/RP-LogicAnalyzer/src/main.cpp > CMakeFiles/crossTest.dir/src/main.cpp.i

CMakeFiles/crossTest.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crossTest.dir/src/main.cpp.s"
	/home/michael/eclipse-workspace/RP-LogicAnalyzer/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/eclipse-workspace/RP-LogicAnalyzer/src/main.cpp -o CMakeFiles/crossTest.dir/src/main.cpp.s

# Object files for target crossTest
crossTest_OBJECTS = \
"CMakeFiles/crossTest.dir/src/main.cpp.o"

# External object files for target crossTest
crossTest_EXTERNAL_OBJECTS =

crossTest: CMakeFiles/crossTest.dir/src/main.cpp.o
crossTest: CMakeFiles/crossTest.dir/build.make
crossTest: ../../externalRedpitaya/lib/librp.a
crossTest: ../../externalRedpitaya/lib/libglib-2.0.so
crossTest: ../../externalRedpitaya/lib/libpython3.5m.so
crossTest: ../../externalRedpitaya/lib/libsigrokdecode.so
crossTest: ../../externalRedpitaya/lib/arm-linux-gnueabihf/libpcre.so
crossTest: ../../externalRedpitaya/lib/arm-linux-gnueabihf/libexpat.so
crossTest: ../../externalRedpitaya/lib/arm-linux-gnueabihf/libz.so
crossTest: ../../externalRedpitaya/lib/libloguru.a
crossTest: CMakeFiles/crossTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/michael/eclipse-workspace/RP-LogicAnalyzer/build/default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable crossTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/crossTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/crossTest.dir/build: crossTest

.PHONY : CMakeFiles/crossTest.dir/build

CMakeFiles/crossTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/crossTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/crossTest.dir/clean

CMakeFiles/crossTest.dir/depend:
	cd /home/michael/eclipse-workspace/RP-LogicAnalyzer/build/default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/michael/eclipse-workspace/RP-LogicAnalyzer /home/michael/eclipse-workspace/RP-LogicAnalyzer /home/michael/eclipse-workspace/RP-LogicAnalyzer/build/default /home/michael/eclipse-workspace/RP-LogicAnalyzer/build/default /home/michael/eclipse-workspace/RP-LogicAnalyzer/build/default/CMakeFiles/crossTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/crossTest.dir/depend

