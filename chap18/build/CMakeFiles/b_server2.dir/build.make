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
CMAKE_SOURCE_DIR = /home/manu/HTTPserver/chap18

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manu/HTTPserver/chap18/build

# Include any dependencies generated for this target.
include CMakeFiles/b_server2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/b_server2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/b_server2.dir/flags.make

CMakeFiles/b_server2.dir/buffer_server2.c.o: CMakeFiles/b_server2.dir/flags.make
CMakeFiles/b_server2.dir/buffer_server2.c.o: ../buffer_server2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manu/HTTPserver/chap18/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/b_server2.dir/buffer_server2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/b_server2.dir/buffer_server2.c.o   -c /home/manu/HTTPserver/chap18/buffer_server2.c

CMakeFiles/b_server2.dir/buffer_server2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/b_server2.dir/buffer_server2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manu/HTTPserver/chap18/buffer_server2.c > CMakeFiles/b_server2.dir/buffer_server2.c.i

CMakeFiles/b_server2.dir/buffer_server2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/b_server2.dir/buffer_server2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manu/HTTPserver/chap18/buffer_server2.c -o CMakeFiles/b_server2.dir/buffer_server2.c.s

# Object files for target b_server2
b_server2_OBJECTS = \
"CMakeFiles/b_server2.dir/buffer_server2.c.o"

# External object files for target b_server2
b_server2_EXTERNAL_OBJECTS =

b_server2: CMakeFiles/b_server2.dir/buffer_server2.c.o
b_server2: CMakeFiles/b_server2.dir/build.make
b_server2: CMakeFiles/b_server2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manu/HTTPserver/chap18/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable b_server2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/b_server2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/b_server2.dir/build: b_server2

.PHONY : CMakeFiles/b_server2.dir/build

CMakeFiles/b_server2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/b_server2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/b_server2.dir/clean

CMakeFiles/b_server2.dir/depend:
	cd /home/manu/HTTPserver/chap18/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manu/HTTPserver/chap18 /home/manu/HTTPserver/chap18 /home/manu/HTTPserver/chap18/build /home/manu/HTTPserver/chap18/build /home/manu/HTTPserver/chap18/build/CMakeFiles/b_server2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/b_server2.dir/depend

