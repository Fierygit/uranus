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
CMAKE_SOURCE_DIR = /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/epoll.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/epoll.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/epoll.dir/flags.make

CMakeFiles/epoll.dir/main.cpp.o: CMakeFiles/epoll.dir/flags.make
CMakeFiles/epoll.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/epoll.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epoll.dir/main.cpp.o -c /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/main.cpp

CMakeFiles/epoll.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epoll.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/main.cpp > CMakeFiles/epoll.dir/main.cpp.i

CMakeFiles/epoll.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epoll.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/main.cpp -o CMakeFiles/epoll.dir/main.cpp.s

CMakeFiles/epoll.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/epoll.dir/main.cpp.o.requires

CMakeFiles/epoll.dir/main.cpp.o.provides: CMakeFiles/epoll.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/epoll.dir/build.make CMakeFiles/epoll.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/epoll.dir/main.cpp.o.provides

CMakeFiles/epoll.dir/main.cpp.o.provides.build: CMakeFiles/epoll.dir/main.cpp.o


CMakeFiles/epoll.dir/http_server.cpp.o: CMakeFiles/epoll.dir/flags.make
CMakeFiles/epoll.dir/http_server.cpp.o: ../http_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/epoll.dir/http_server.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epoll.dir/http_server.cpp.o -c /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/http_server.cpp

CMakeFiles/epoll.dir/http_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epoll.dir/http_server.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/http_server.cpp > CMakeFiles/epoll.dir/http_server.cpp.i

CMakeFiles/epoll.dir/http_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epoll.dir/http_server.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/http_server.cpp -o CMakeFiles/epoll.dir/http_server.cpp.s

CMakeFiles/epoll.dir/http_server.cpp.o.requires:

.PHONY : CMakeFiles/epoll.dir/http_server.cpp.o.requires

CMakeFiles/epoll.dir/http_server.cpp.o.provides: CMakeFiles/epoll.dir/http_server.cpp.o.requires
	$(MAKE) -f CMakeFiles/epoll.dir/build.make CMakeFiles/epoll.dir/http_server.cpp.o.provides.build
.PHONY : CMakeFiles/epoll.dir/http_server.cpp.o.provides

CMakeFiles/epoll.dir/http_server.cpp.o.provides.build: CMakeFiles/epoll.dir/http_server.cpp.o


CMakeFiles/epoll.dir/handler.cpp.o: CMakeFiles/epoll.dir/flags.make
CMakeFiles/epoll.dir/handler.cpp.o: ../handler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/epoll.dir/handler.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epoll.dir/handler.cpp.o -c /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/handler.cpp

CMakeFiles/epoll.dir/handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epoll.dir/handler.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/handler.cpp > CMakeFiles/epoll.dir/handler.cpp.i

CMakeFiles/epoll.dir/handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epoll.dir/handler.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/handler.cpp -o CMakeFiles/epoll.dir/handler.cpp.s

CMakeFiles/epoll.dir/handler.cpp.o.requires:

.PHONY : CMakeFiles/epoll.dir/handler.cpp.o.requires

CMakeFiles/epoll.dir/handler.cpp.o.provides: CMakeFiles/epoll.dir/handler.cpp.o.requires
	$(MAKE) -f CMakeFiles/epoll.dir/build.make CMakeFiles/epoll.dir/handler.cpp.o.provides.build
.PHONY : CMakeFiles/epoll.dir/handler.cpp.o.provides

CMakeFiles/epoll.dir/handler.cpp.o.provides.build: CMakeFiles/epoll.dir/handler.cpp.o


CMakeFiles/epoll.dir/debug_log.cpp.o: CMakeFiles/epoll.dir/flags.make
CMakeFiles/epoll.dir/debug_log.cpp.o: ../debug_log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/epoll.dir/debug_log.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epoll.dir/debug_log.cpp.o -c /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/debug_log.cpp

CMakeFiles/epoll.dir/debug_log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epoll.dir/debug_log.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/debug_log.cpp > CMakeFiles/epoll.dir/debug_log.cpp.i

CMakeFiles/epoll.dir/debug_log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epoll.dir/debug_log.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/debug_log.cpp -o CMakeFiles/epoll.dir/debug_log.cpp.s

CMakeFiles/epoll.dir/debug_log.cpp.o.requires:

.PHONY : CMakeFiles/epoll.dir/debug_log.cpp.o.requires

CMakeFiles/epoll.dir/debug_log.cpp.o.provides: CMakeFiles/epoll.dir/debug_log.cpp.o.requires
	$(MAKE) -f CMakeFiles/epoll.dir/build.make CMakeFiles/epoll.dir/debug_log.cpp.o.provides.build
.PHONY : CMakeFiles/epoll.dir/debug_log.cpp.o.provides

CMakeFiles/epoll.dir/debug_log.cpp.o.provides.build: CMakeFiles/epoll.dir/debug_log.cpp.o


# Object files for target epoll
epoll_OBJECTS = \
"CMakeFiles/epoll.dir/main.cpp.o" \
"CMakeFiles/epoll.dir/http_server.cpp.o" \
"CMakeFiles/epoll.dir/handler.cpp.o" \
"CMakeFiles/epoll.dir/debug_log.cpp.o"

# External object files for target epoll
epoll_EXTERNAL_OBJECTS =

epoll: CMakeFiles/epoll.dir/main.cpp.o
epoll: CMakeFiles/epoll.dir/http_server.cpp.o
epoll: CMakeFiles/epoll.dir/handler.cpp.o
epoll: CMakeFiles/epoll.dir/debug_log.cpp.o
epoll: CMakeFiles/epoll.dir/build.make
epoll: CMakeFiles/epoll.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable epoll"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/epoll.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/epoll.dir/build: epoll

.PHONY : CMakeFiles/epoll.dir/build

CMakeFiles/epoll.dir/requires: CMakeFiles/epoll.dir/main.cpp.o.requires
CMakeFiles/epoll.dir/requires: CMakeFiles/epoll.dir/http_server.cpp.o.requires
CMakeFiles/epoll.dir/requires: CMakeFiles/epoll.dir/handler.cpp.o.requires
CMakeFiles/epoll.dir/requires: CMakeFiles/epoll.dir/debug_log.cpp.o.requires

.PHONY : CMakeFiles/epoll.dir/requires

CMakeFiles/epoll.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/epoll.dir/cmake_clean.cmake
.PHONY : CMakeFiles/epoll.dir/clean

CMakeFiles/epoll.dir/depend:
	cd /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug /mnt/e/virtualShare/gopath3/src/uranus/Lab2/epoll/cmake-build-debug/CMakeFiles/epoll.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/epoll.dir/depend

