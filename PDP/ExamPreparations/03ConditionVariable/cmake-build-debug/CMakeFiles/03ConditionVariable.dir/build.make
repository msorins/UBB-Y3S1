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
CMAKE_SOURCE_DIR = /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/03ConditionVariable.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/03ConditionVariable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/03ConditionVariable.dir/flags.make

CMakeFiles/03ConditionVariable.dir/main.cpp.o: CMakeFiles/03ConditionVariable.dir/flags.make
CMakeFiles/03ConditionVariable.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/03ConditionVariable.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/03ConditionVariable.dir/main.cpp.o -c /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/main.cpp

CMakeFiles/03ConditionVariable.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/03ConditionVariable.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/main.cpp > CMakeFiles/03ConditionVariable.dir/main.cpp.i

CMakeFiles/03ConditionVariable.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/03ConditionVariable.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/main.cpp -o CMakeFiles/03ConditionVariable.dir/main.cpp.s

# Object files for target 03ConditionVariable
03ConditionVariable_OBJECTS = \
"CMakeFiles/03ConditionVariable.dir/main.cpp.o"

# External object files for target 03ConditionVariable
03ConditionVariable_EXTERNAL_OBJECTS =

03ConditionVariable: CMakeFiles/03ConditionVariable.dir/main.cpp.o
03ConditionVariable: CMakeFiles/03ConditionVariable.dir/build.make
03ConditionVariable: CMakeFiles/03ConditionVariable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 03ConditionVariable"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/03ConditionVariable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/03ConditionVariable.dir/build: 03ConditionVariable

.PHONY : CMakeFiles/03ConditionVariable.dir/build

CMakeFiles/03ConditionVariable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/03ConditionVariable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/03ConditionVariable.dir/clean

CMakeFiles/03ConditionVariable.dir/depend:
	cd /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/cmake-build-debug /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/cmake-build-debug /Users/so/Desktop/Y3S1/PDP/ExamPreparations/03ConditionVariable/cmake-build-debug/CMakeFiles/03ConditionVariable.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/03ConditionVariable.dir/depend

