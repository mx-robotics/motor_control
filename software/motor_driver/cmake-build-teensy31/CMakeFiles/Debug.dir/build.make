# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /snap/clion/124/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/124/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/firat/projects/motor_control/software/motor_driver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/firat/projects/motor_control/software/motor_driver/cmake-build-teensy31

# Utility rule file for Debug.

# Include the progress variables for this target.
include CMakeFiles/Debug.dir/progress.make

CMakeFiles/Debug:
	cd /home/firat/projects/motor_control/software/motor_driver && platformio -c clion run --target debug -eteensy31

Debug: CMakeFiles/Debug
Debug: CMakeFiles/Debug.dir/build.make

.PHONY : Debug

# Rule to build all files generated by this target.
CMakeFiles/Debug.dir/build: Debug

.PHONY : CMakeFiles/Debug.dir/build

CMakeFiles/Debug.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Debug.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Debug.dir/clean

CMakeFiles/Debug.dir/depend:
	cd /home/firat/projects/motor_control/software/motor_driver/cmake-build-teensy31 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/firat/projects/motor_control/software/motor_driver /home/firat/projects/motor_control/software/motor_driver /home/firat/projects/motor_control/software/motor_driver/cmake-build-teensy31 /home/firat/projects/motor_control/software/motor_driver/cmake-build-teensy31 /home/firat/projects/motor_control/software/motor_driver/cmake-build-teensy31/CMakeFiles/Debug.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Debug.dir/depend

