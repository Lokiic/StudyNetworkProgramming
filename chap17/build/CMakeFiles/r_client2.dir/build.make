# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/manu/HTTPserver/chap17

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manu/HTTPserver/chap17/build

# Include any dependencies generated for this target.
include CMakeFiles/r_client2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/r_client2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/r_client2.dir/flags.make

CMakeFiles/r_client2.dir/reliable_client02.c.o: CMakeFiles/r_client2.dir/flags.make
CMakeFiles/r_client2.dir/reliable_client02.c.o: ../reliable_client02.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manu/HTTPserver/chap17/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/r_client2.dir/reliable_client02.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/r_client2.dir/reliable_client02.c.o   -c /home/manu/HTTPserver/chap17/reliable_client02.c

CMakeFiles/r_client2.dir/reliable_client02.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/r_client2.dir/reliable_client02.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manu/HTTPserver/chap17/reliable_client02.c > CMakeFiles/r_client2.dir/reliable_client02.c.i

CMakeFiles/r_client2.dir/reliable_client02.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/r_client2.dir/reliable_client02.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manu/HTTPserver/chap17/reliable_client02.c -o CMakeFiles/r_client2.dir/reliable_client02.c.s

# Object files for target r_client2
r_client2_OBJECTS = \
"CMakeFiles/r_client2.dir/reliable_client02.c.o"

# External object files for target r_client2
r_client2_EXTERNAL_OBJECTS =

r_client2: CMakeFiles/r_client2.dir/reliable_client02.c.o
r_client2: CMakeFiles/r_client2.dir/build.make
r_client2: CMakeFiles/r_client2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manu/HTTPserver/chap17/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable r_client2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/r_client2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/r_client2.dir/build: r_client2

.PHONY : CMakeFiles/r_client2.dir/build

CMakeFiles/r_client2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/r_client2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/r_client2.dir/clean

CMakeFiles/r_client2.dir/depend:
	cd /home/manu/HTTPserver/chap17/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manu/HTTPserver/chap17 /home/manu/HTTPserver/chap17 /home/manu/HTTPserver/chap17/build /home/manu/HTTPserver/chap17/build /home/manu/HTTPserver/chap17/build/CMakeFiles/r_client2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/r_client2.dir/depend
