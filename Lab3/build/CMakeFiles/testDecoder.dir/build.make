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
include CMakeFiles/testDecoder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testDecoder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testDecoder.dir/flags.make

CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o: CMakeFiles/testDecoder.dir/flags.make
CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o: ../test/testDecoder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o -c /mnt/e/virtualShare/gopath3/src/uranus/Lab3/test/testDecoder.cpp

CMakeFiles/testDecoder.dir/test/testDecoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testDecoder.dir/test/testDecoder.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/virtualShare/gopath3/src/uranus/Lab3/test/testDecoder.cpp > CMakeFiles/testDecoder.dir/test/testDecoder.cpp.i

CMakeFiles/testDecoder.dir/test/testDecoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testDecoder.dir/test/testDecoder.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/virtualShare/gopath3/src/uranus/Lab3/test/testDecoder.cpp -o CMakeFiles/testDecoder.dir/test/testDecoder.cpp.s

CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.requires:

.PHONY : CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.requires

CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.provides: CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.requires
	$(MAKE) -f CMakeFiles/testDecoder.dir/build.make CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.provides.build
.PHONY : CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.provides

CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.provides.build: CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o


# Object files for target testDecoder
testDecoder_OBJECTS = \
"CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o"

# External object files for target testDecoder
testDecoder_EXTERNAL_OBJECTS =

testDecoder: CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o
testDecoder: CMakeFiles/testDecoder.dir/build.make
testDecoder: CMakeFiles/testDecoder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testDecoder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testDecoder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testDecoder.dir/build: testDecoder

.PHONY : CMakeFiles/testDecoder.dir/build

CMakeFiles/testDecoder.dir/requires: CMakeFiles/testDecoder.dir/test/testDecoder.cpp.o.requires

.PHONY : CMakeFiles/testDecoder.dir/requires

CMakeFiles/testDecoder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testDecoder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testDecoder.dir/clean

CMakeFiles/testDecoder.dir/depend:
	cd /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/virtualShare/gopath3/src/uranus/Lab3 /mnt/e/virtualShare/gopath3/src/uranus/Lab3 /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build /mnt/e/virtualShare/gopath3/src/uranus/Lab3/build/CMakeFiles/testDecoder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testDecoder.dir/depend

