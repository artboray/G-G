# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/borat/CLionProjects/FORGIT/geom3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/borat/CLionProjects/FORGIT/geom3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/geom3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/geom3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/geom3.dir/flags.make

CMakeFiles/geom3.dir/main.cpp.o: CMakeFiles/geom3.dir/flags.make
CMakeFiles/geom3.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/borat/CLionProjects/FORGIT/geom3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/geom3.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/geom3.dir/main.cpp.o -c /Users/borat/CLionProjects/FORGIT/geom3/main.cpp

CMakeFiles/geom3.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/geom3.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/borat/CLionProjects/FORGIT/geom3/main.cpp > CMakeFiles/geom3.dir/main.cpp.i

CMakeFiles/geom3.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/geom3.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/borat/CLionProjects/FORGIT/geom3/main.cpp -o CMakeFiles/geom3.dir/main.cpp.s

CMakeFiles/geom3.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/geom3.dir/main.cpp.o.requires

CMakeFiles/geom3.dir/main.cpp.o.provides: CMakeFiles/geom3.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/geom3.dir/build.make CMakeFiles/geom3.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/geom3.dir/main.cpp.o.provides

CMakeFiles/geom3.dir/main.cpp.o.provides.build: CMakeFiles/geom3.dir/main.cpp.o


# Object files for target geom3
geom3_OBJECTS = \
"CMakeFiles/geom3.dir/main.cpp.o"

# External object files for target geom3
geom3_EXTERNAL_OBJECTS =

geom3: CMakeFiles/geom3.dir/main.cpp.o
geom3: CMakeFiles/geom3.dir/build.make
geom3: CMakeFiles/geom3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/borat/CLionProjects/FORGIT/geom3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable geom3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/geom3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/geom3.dir/build: geom3

.PHONY : CMakeFiles/geom3.dir/build

CMakeFiles/geom3.dir/requires: CMakeFiles/geom3.dir/main.cpp.o.requires

.PHONY : CMakeFiles/geom3.dir/requires

CMakeFiles/geom3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/geom3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/geom3.dir/clean

CMakeFiles/geom3.dir/depend:
	cd /Users/borat/CLionProjects/FORGIT/geom3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/borat/CLionProjects/FORGIT/geom3 /Users/borat/CLionProjects/FORGIT/geom3 /Users/borat/CLionProjects/FORGIT/geom3/cmake-build-debug /Users/borat/CLionProjects/FORGIT/geom3/cmake-build-debug /Users/borat/CLionProjects/FORGIT/geom3/cmake-build-debug/CMakeFiles/geom3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/geom3.dir/depend

