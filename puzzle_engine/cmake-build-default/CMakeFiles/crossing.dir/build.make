# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\cmake-build-default

# Include any dependencies generated for this target.
include CMakeFiles/crossing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/crossing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/crossing.dir/flags.make

CMakeFiles/crossing.dir/crossing.cpp.obj: CMakeFiles/crossing.dir/flags.make
CMakeFiles/crossing.dir/crossing.cpp.obj: ../crossing.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\cmake-build-default\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/crossing.dir/crossing.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\crossing.dir\crossing.cpp.obj -c C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\crossing.cpp

CMakeFiles/crossing.dir/crossing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crossing.dir/crossing.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\crossing.cpp > CMakeFiles\crossing.dir\crossing.cpp.i

CMakeFiles/crossing.dir/crossing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crossing.dir/crossing.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\crossing.cpp -o CMakeFiles\crossing.dir\crossing.cpp.s

# Object files for target crossing
crossing_OBJECTS = \
"CMakeFiles/crossing.dir/crossing.cpp.obj"

# External object files for target crossing
crossing_EXTERNAL_OBJECTS =

crossing.exe: CMakeFiles/crossing.dir/crossing.cpp.obj
crossing.exe: CMakeFiles/crossing.dir/build.make
crossing.exe: CMakeFiles/crossing.dir/linklibs.rsp
crossing.exe: CMakeFiles/crossing.dir/objects1.rsp
crossing.exe: CMakeFiles/crossing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\cmake-build-default\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable crossing.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\crossing.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/crossing.dir/build: crossing.exe

.PHONY : CMakeFiles/crossing.dir/build

CMakeFiles/crossing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\crossing.dir\cmake_clean.cmake
.PHONY : CMakeFiles/crossing.dir/clean

CMakeFiles/crossing.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\cmake-build-default C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\cmake-build-default C:\Users\Nille\OneDrive\Skole\Filer\P8\AP\puzzle_engine\cmake-build-default\CMakeFiles\crossing.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/crossing.dir/depend

