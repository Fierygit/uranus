# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /mnt/e/virtualShare/gopath3/src/uranus/Lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build

# Include any dependencies generated for this target.
include CMakeFiles/testEncoder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testEncoder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testEncoder.dir/flags.make

CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o: CMakeFiles/testEncoder.dir/flags.make
CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o: ../test/testEncoder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o -c /mnt/e/virtualShare/gopath3/src/uranus/Lab3/test/testEncoder.cpp

CMakeFiles/testEncoder.dir/test/testEncoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testEncoder.dir/test/testEncoder.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/virtualShare/gopath3/src/uranus/Lab3/test/testEncoder.cpp > CMakeFiles/testEncoder.dir/test/testEncoder.cpp.i

CMakeFiles/testEncoder.dir/test/testEncoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testEncoder.dir/test/testEncoder.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/virtualShare/gopath3/src/uranus/Lab3/test/testEncoder.cpp -o CMakeFiles/testEncoder.dir/test/testEncoder.cpp.s

CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.requires:

.PHONY : CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.requires

CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.provides: CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.requires
	$(MAKE) -f CMakeFiles/testEncoder.dir/build.make CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.provides.build
.PHONY : CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.provides

CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.provides.build: CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o


# Object files for target testEncoder
testEncoder_OBJECTS = \
"CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o"

# External object files for target testEncoder
testEncoder_EXTERNAL_OBJECTS =

testEncoder: CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o
testEncoder: CMakeFiles/testEncoder.dir/build.make
testEncoder: CMakeFiles/testEncoder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testEncoder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testEncoder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testEncoder.dir/build: testEncoder

.PHONY : CMakeFiles/testEncoder.dir/build

CMakeFiles/testEncoder.dir/requires: CMakeFiles/testEncoder.dir/test/testEncoder.cpp.o.requires

.PHONY : CMakeFiles/testEncoder.dir/requires

CMakeFiles/testEncoder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testEncoder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testEncoder.dir/clean

CMakeFiles/testEncoder.dir/depend:
	cd /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/virtualShare/gopath3/src/uranus/Lab3 /mnt/e/virtualShare/gopath3/src/uranus/Lab3 /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build/CMakeFiles/testEncoder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testEncoder.dir/depend

