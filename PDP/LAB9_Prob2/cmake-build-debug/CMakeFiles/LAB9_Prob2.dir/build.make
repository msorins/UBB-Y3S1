# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/LAB9_Prob2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/LAB9_Prob2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LAB9_Prob2.dir/flags.make

CMakeFiles/LAB9_Prob2.dir/main.cpp.o: CMakeFiles/LAB9_Prob2.dir/flags.make
CMakeFiles/LAB9_Prob2.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LAB9_Prob2.dir/main.cpp.o"
	mpicxx  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LAB9_Prob2.dir/main.cpp.o -c /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/main.cpp

CMakeFiles/LAB9_Prob2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LAB9_Prob2.dir/main.cpp.i"
	mpicxx $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/main.cpp > CMakeFiles/LAB9_Prob2.dir/main.cpp.i

CMakeFiles/LAB9_Prob2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LAB9_Prob2.dir/main.cpp.s"
	mpicxx $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/main.cpp -o CMakeFiles/LAB9_Prob2.dir/main.cpp.s

# Object files for target LAB9_Prob2
LAB9_Prob2_OBJECTS = \
"CMakeFiles/LAB9_Prob2.dir/main.cpp.o"

# External object files for target LAB9_Prob2
LAB9_Prob2_EXTERNAL_OBJECTS =

LAB9_Prob2: CMakeFiles/LAB9_Prob2.dir/main.cpp.o
LAB9_Prob2: CMakeFiles/LAB9_Prob2.dir/build.make
LAB9_Prob2: CMakeFiles/LAB9_Prob2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable LAB9_Prob2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LAB9_Prob2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LAB9_Prob2.dir/build: LAB9_Prob2

.PHONY : CMakeFiles/LAB9_Prob2.dir/build

CMakeFiles/LAB9_Prob2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LAB9_Prob2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LAB9_Prob2.dir/clean

CMakeFiles/LAB9_Prob2.dir/depend:
	cd /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2 /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2 /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/cmake-build-debug /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/cmake-build-debug /Users/so/Desktop/Y3S1/PDP/LAB9_Prob2/cmake-build-debug/CMakeFiles/LAB9_Prob2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LAB9_Prob2.dir/depend
