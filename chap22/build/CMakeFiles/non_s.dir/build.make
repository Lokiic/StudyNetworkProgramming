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
CMAKE_SOURCE_DIR = /home/manu/HTTPserver/chap22

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manu/HTTPserver/chap22/build

# Include any dependencies generated for this target.
include CMakeFiles/non_s.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/non_s.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/non_s.dir/flags.make

CMakeFiles/non_s.dir/nonblocking_server.c.o: CMakeFiles/non_s.dir/flags.make
CMakeFiles/non_s.dir/nonblocking_server.c.o: ../nonblocking_server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manu/HTTPserver/chap22/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/non_s.dir/nonblocking_server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/non_s.dir/nonblocking_server.c.o   -c /home/manu/HTTPserver/chap22/nonblocking_server.c

CMakeFiles/non_s.dir/nonblocking_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/non_s.dir/nonblocking_server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manu/HTTPserver/chap22/nonblocking_server.c > CMakeFiles/non_s.dir/nonblocking_server.c.i

CMakeFiles/non_s.dir/nonblocking_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/non_s.dir/nonblocking_server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manu/HTTPserver/chap22/nonblocking_server.c -o CMakeFiles/non_s.dir/nonblocking_server.c.s

# Object files for target non_s
non_s_OBJECTS = \
"CMakeFiles/non_s.dir/nonblocking_server.c.o"

# External object files for target non_s
non_s_EXTERNAL_OBJECTS =

non_s: CMakeFiles/non_s.dir/nonblocking_server.c.o
non_s: CMakeFiles/non_s.dir/build.make
non_s: CMakeFiles/non_s.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manu/HTTPserver/chap22/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable non_s"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/non_s.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/non_s.dir/build: non_s

.PHONY : CMakeFiles/non_s.dir/build

CMakeFiles/non_s.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/non_s.dir/cmake_clean.cmake
.PHONY : CMakeFiles/non_s.dir/clean

CMakeFiles/non_s.dir/depend:
	cd /home/manu/HTTPserver/chap22/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manu/HTTPserver/chap22 /home/manu/HTTPserver/chap22 /home/manu/HTTPserver/chap22/build /home/manu/HTTPserver/chap22/build /home/manu/HTTPserver/chap22/build/CMakeFiles/non_s.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/non_s.dir/depend

