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
CMAKE_SOURCE_DIR = /home/manu/HTTPserver/chap26

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manu/HTTPserver/chap26/build

# Include any dependencies generated for this target.
include CMakeFiles/thread_pool.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/thread_pool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/thread_pool.dir/flags.make

CMakeFiles/thread_pool.dir/Thread_pool.c.o: CMakeFiles/thread_pool.dir/flags.make
CMakeFiles/thread_pool.dir/Thread_pool.c.o: ../Thread_pool.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manu/HTTPserver/chap26/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/thread_pool.dir/Thread_pool.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/thread_pool.dir/Thread_pool.c.o   -c /home/manu/HTTPserver/chap26/Thread_pool.c

CMakeFiles/thread_pool.dir/Thread_pool.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/thread_pool.dir/Thread_pool.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manu/HTTPserver/chap26/Thread_pool.c > CMakeFiles/thread_pool.dir/Thread_pool.c.i

CMakeFiles/thread_pool.dir/Thread_pool.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/thread_pool.dir/Thread_pool.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manu/HTTPserver/chap26/Thread_pool.c -o CMakeFiles/thread_pool.dir/Thread_pool.c.s

CMakeFiles/thread_pool.dir/Echo.c.o: CMakeFiles/thread_pool.dir/flags.make
CMakeFiles/thread_pool.dir/Echo.c.o: ../Echo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manu/HTTPserver/chap26/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/thread_pool.dir/Echo.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/thread_pool.dir/Echo.c.o   -c /home/manu/HTTPserver/chap26/Echo.c

CMakeFiles/thread_pool.dir/Echo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/thread_pool.dir/Echo.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manu/HTTPserver/chap26/Echo.c > CMakeFiles/thread_pool.dir/Echo.c.i

CMakeFiles/thread_pool.dir/Echo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/thread_pool.dir/Echo.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manu/HTTPserver/chap26/Echo.c -o CMakeFiles/thread_pool.dir/Echo.c.s

# Object files for target thread_pool
thread_pool_OBJECTS = \
"CMakeFiles/thread_pool.dir/Thread_pool.c.o" \
"CMakeFiles/thread_pool.dir/Echo.c.o"

# External object files for target thread_pool
thread_pool_EXTERNAL_OBJECTS =

thread_pool: CMakeFiles/thread_pool.dir/Thread_pool.c.o
thread_pool: CMakeFiles/thread_pool.dir/Echo.c.o
thread_pool: CMakeFiles/thread_pool.dir/build.make
thread_pool: CMakeFiles/thread_pool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manu/HTTPserver/chap26/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable thread_pool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/thread_pool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/thread_pool.dir/build: thread_pool

.PHONY : CMakeFiles/thread_pool.dir/build

CMakeFiles/thread_pool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/thread_pool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/thread_pool.dir/clean

CMakeFiles/thread_pool.dir/depend:
	cd /home/manu/HTTPserver/chap26/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manu/HTTPserver/chap26 /home/manu/HTTPserver/chap26 /home/manu/HTTPserver/chap26/build /home/manu/HTTPserver/chap26/build /home/manu/HTTPserver/chap26/build/CMakeFiles/thread_pool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/thread_pool.dir/depend

