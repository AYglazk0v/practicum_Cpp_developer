# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gtaggana/Task

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gtaggana/Task/build

# Include any dependencies generated for this target.
include CMakeFiles/search_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/search_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/search_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/search_server.dir/flags.make

CMakeFiles/search_server.dir/document.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/document.cpp.o: ../document.cpp
CMakeFiles/search_server.dir/document.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/search_server.dir/document.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/document.cpp.o -MF CMakeFiles/search_server.dir/document.cpp.o.d -o CMakeFiles/search_server.dir/document.cpp.o -c /Users/gtaggana/Task/document.cpp

CMakeFiles/search_server.dir/document.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/document.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/document.cpp > CMakeFiles/search_server.dir/document.cpp.i

CMakeFiles/search_server.dir/document.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/document.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/document.cpp -o CMakeFiles/search_server.dir/document.cpp.s

CMakeFiles/search_server.dir/main.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/main.cpp.o: ../main.cpp
CMakeFiles/search_server.dir/main.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/search_server.dir/main.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/main.cpp.o -MF CMakeFiles/search_server.dir/main.cpp.o.d -o CMakeFiles/search_server.dir/main.cpp.o -c /Users/gtaggana/Task/main.cpp

CMakeFiles/search_server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/main.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/main.cpp > CMakeFiles/search_server.dir/main.cpp.i

CMakeFiles/search_server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/main.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/main.cpp -o CMakeFiles/search_server.dir/main.cpp.s

CMakeFiles/search_server.dir/process_queries.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/process_queries.cpp.o: ../process_queries.cpp
CMakeFiles/search_server.dir/process_queries.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/search_server.dir/process_queries.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/process_queries.cpp.o -MF CMakeFiles/search_server.dir/process_queries.cpp.o.d -o CMakeFiles/search_server.dir/process_queries.cpp.o -c /Users/gtaggana/Task/process_queries.cpp

CMakeFiles/search_server.dir/process_queries.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/process_queries.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/process_queries.cpp > CMakeFiles/search_server.dir/process_queries.cpp.i

CMakeFiles/search_server.dir/process_queries.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/process_queries.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/process_queries.cpp -o CMakeFiles/search_server.dir/process_queries.cpp.s

CMakeFiles/search_server.dir/profiling.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/profiling.cpp.o: ../profiling.cpp
CMakeFiles/search_server.dir/profiling.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/search_server.dir/profiling.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/profiling.cpp.o -MF CMakeFiles/search_server.dir/profiling.cpp.o.d -o CMakeFiles/search_server.dir/profiling.cpp.o -c /Users/gtaggana/Task/profiling.cpp

CMakeFiles/search_server.dir/profiling.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/profiling.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/profiling.cpp > CMakeFiles/search_server.dir/profiling.cpp.i

CMakeFiles/search_server.dir/profiling.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/profiling.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/profiling.cpp -o CMakeFiles/search_server.dir/profiling.cpp.s

CMakeFiles/search_server.dir/read_input_functions.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/read_input_functions.cpp.o: ../read_input_functions.cpp
CMakeFiles/search_server.dir/read_input_functions.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/search_server.dir/read_input_functions.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/read_input_functions.cpp.o -MF CMakeFiles/search_server.dir/read_input_functions.cpp.o.d -o CMakeFiles/search_server.dir/read_input_functions.cpp.o -c /Users/gtaggana/Task/read_input_functions.cpp

CMakeFiles/search_server.dir/read_input_functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/read_input_functions.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/read_input_functions.cpp > CMakeFiles/search_server.dir/read_input_functions.cpp.i

CMakeFiles/search_server.dir/read_input_functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/read_input_functions.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/read_input_functions.cpp -o CMakeFiles/search_server.dir/read_input_functions.cpp.s

CMakeFiles/search_server.dir/remove_duplicates.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/remove_duplicates.cpp.o: ../remove_duplicates.cpp
CMakeFiles/search_server.dir/remove_duplicates.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/search_server.dir/remove_duplicates.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/remove_duplicates.cpp.o -MF CMakeFiles/search_server.dir/remove_duplicates.cpp.o.d -o CMakeFiles/search_server.dir/remove_duplicates.cpp.o -c /Users/gtaggana/Task/remove_duplicates.cpp

CMakeFiles/search_server.dir/remove_duplicates.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/remove_duplicates.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/remove_duplicates.cpp > CMakeFiles/search_server.dir/remove_duplicates.cpp.i

CMakeFiles/search_server.dir/remove_duplicates.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/remove_duplicates.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/remove_duplicates.cpp -o CMakeFiles/search_server.dir/remove_duplicates.cpp.s

CMakeFiles/search_server.dir/request_queue.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/request_queue.cpp.o: ../request_queue.cpp
CMakeFiles/search_server.dir/request_queue.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/search_server.dir/request_queue.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/request_queue.cpp.o -MF CMakeFiles/search_server.dir/request_queue.cpp.o.d -o CMakeFiles/search_server.dir/request_queue.cpp.o -c /Users/gtaggana/Task/request_queue.cpp

CMakeFiles/search_server.dir/request_queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/request_queue.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/request_queue.cpp > CMakeFiles/search_server.dir/request_queue.cpp.i

CMakeFiles/search_server.dir/request_queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/request_queue.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/request_queue.cpp -o CMakeFiles/search_server.dir/request_queue.cpp.s

CMakeFiles/search_server.dir/search_server.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/search_server.cpp.o: ../search_server.cpp
CMakeFiles/search_server.dir/search_server.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/search_server.dir/search_server.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/search_server.cpp.o -MF CMakeFiles/search_server.dir/search_server.cpp.o.d -o CMakeFiles/search_server.dir/search_server.cpp.o -c /Users/gtaggana/Task/search_server.cpp

CMakeFiles/search_server.dir/search_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/search_server.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/search_server.cpp > CMakeFiles/search_server.dir/search_server.cpp.i

CMakeFiles/search_server.dir/search_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/search_server.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/search_server.cpp -o CMakeFiles/search_server.dir/search_server.cpp.s

CMakeFiles/search_server.dir/string_processing.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/string_processing.cpp.o: ../string_processing.cpp
CMakeFiles/search_server.dir/string_processing.cpp.o: CMakeFiles/search_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/search_server.dir/string_processing.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/search_server.dir/string_processing.cpp.o -MF CMakeFiles/search_server.dir/string_processing.cpp.o.d -o CMakeFiles/search_server.dir/string_processing.cpp.o -c /Users/gtaggana/Task/string_processing.cpp

CMakeFiles/search_server.dir/string_processing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/string_processing.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gtaggana/Task/string_processing.cpp > CMakeFiles/search_server.dir/string_processing.cpp.i

CMakeFiles/search_server.dir/string_processing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/string_processing.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gtaggana/Task/string_processing.cpp -o CMakeFiles/search_server.dir/string_processing.cpp.s

# Object files for target search_server
search_server_OBJECTS = \
"CMakeFiles/search_server.dir/document.cpp.o" \
"CMakeFiles/search_server.dir/main.cpp.o" \
"CMakeFiles/search_server.dir/process_queries.cpp.o" \
"CMakeFiles/search_server.dir/profiling.cpp.o" \
"CMakeFiles/search_server.dir/read_input_functions.cpp.o" \
"CMakeFiles/search_server.dir/remove_duplicates.cpp.o" \
"CMakeFiles/search_server.dir/request_queue.cpp.o" \
"CMakeFiles/search_server.dir/search_server.cpp.o" \
"CMakeFiles/search_server.dir/string_processing.cpp.o"

# External object files for target search_server
search_server_EXTERNAL_OBJECTS =

search_server: CMakeFiles/search_server.dir/document.cpp.o
search_server: CMakeFiles/search_server.dir/main.cpp.o
search_server: CMakeFiles/search_server.dir/process_queries.cpp.o
search_server: CMakeFiles/search_server.dir/profiling.cpp.o
search_server: CMakeFiles/search_server.dir/read_input_functions.cpp.o
search_server: CMakeFiles/search_server.dir/remove_duplicates.cpp.o
search_server: CMakeFiles/search_server.dir/request_queue.cpp.o
search_server: CMakeFiles/search_server.dir/search_server.cpp.o
search_server: CMakeFiles/search_server.dir/string_processing.cpp.o
search_server: CMakeFiles/search_server.dir/build.make
search_server: CMakeFiles/search_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/gtaggana/Task/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable search_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/search_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/search_server.dir/build: search_server
.PHONY : CMakeFiles/search_server.dir/build

CMakeFiles/search_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/search_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/search_server.dir/clean

CMakeFiles/search_server.dir/depend:
	cd /Users/gtaggana/Task/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gtaggana/Task /Users/gtaggana/Task /Users/gtaggana/Task/build /Users/gtaggana/Task/build /Users/gtaggana/Task/build/CMakeFiles/search_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/search_server.dir/depend

