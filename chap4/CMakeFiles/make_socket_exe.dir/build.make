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
CMAKE_SOURCE_DIR = /home/manu/HTTPserver/chap4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manu/HTTPserver/chap4

# Include any dependencies generated for this target.
include CMakeFiles/make_socket_exe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/make_socket_exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/make_socket_exe.dir/flags.make

CMakeFiles/make_socket_exe.dir/make_socket.c.o: CMakeFiles/make_socket_exe.dir/flags.make
CMakeFiles/make_socket_exe.dir/make_socket.c.o: make_socket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manu/HTTPserver/chap4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/make_socket_exe.dir/make_socket.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/make_socket_exe.dir/make_socket.c.o   -c /home/manu/HTTPserver/chap4/make_socket.c

CMakeFiles/make_socket_exe.dir/make_socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/make_socket_exe.dir/make_socket.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manu/HTTPserver/chap4/make_socket.c > CMakeFiles/make_socket_exe.dir/make_socket.c.i

CMakeFiles/make_socket_exe.dir/make_socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/make_socket_exe.dir/make_socket.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manu/HTTPserver/chap4/make_socket.c -o CMakeFiles/make_socket_exe.dir/make_socket.c.s

# Object files for target make_socket_exe
make_socket_exe_OBJECTS = \
"CMakeFiles/make_socket_exe.dir/make_socket.c.o"

# External object files for target make_socket_exe
make_socket_exe_EXTERNAL_OBJECTS =

make_socket_exe: CMakeFiles/make_socket_exe.dir/make_socket.c.o
make_socket_exe: CMakeFiles/make_socket_exe.dir/build.make
make_socket_exe: CMakeFiles/make_socket_exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manu/HTTPserver/chap4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable make_socket_exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/make_socket_exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/make_socket_exe.dir/build: make_socket_exe

.PHONY : CMakeFiles/make_socket_exe.dir/build

CMakeFiles/make_socket_exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/make_socket_exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/make_socket_exe.dir/clean

CMakeFiles/make_socket_exe.dir/depend:
	cd /home/manu/HTTPserver/chap4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manu/HTTPserver/chap4 /home/manu/HTTPserver/chap4 /home/manu/HTTPserver/chap4 /home/manu/HTTPserver/chap4 /home/manu/HTTPserver/chap4/CMakeFiles/make_socket_exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/make_socket_exe.dir/depend

