# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /snap/clion/189/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/189/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug"

# Include any dependencies generated for this target.
include src/CMakeFiles/sockpp-objs.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/sockpp-objs.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/sockpp-objs.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/sockpp-objs.dir/flags.make

src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.o: ../src/acceptor.cpp
src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.o -MF CMakeFiles/sockpp-objs.dir/acceptor.cpp.o.d -o CMakeFiles/sockpp-objs.dir/acceptor.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/acceptor.cpp"

src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/acceptor.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/acceptor.cpp" > CMakeFiles/sockpp-objs.dir/acceptor.cpp.i

src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/acceptor.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/acceptor.cpp" -o CMakeFiles/sockpp-objs.dir/acceptor.cpp.s

src/CMakeFiles/sockpp-objs.dir/connector.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/connector.cpp.o: ../src/connector.cpp
src/CMakeFiles/sockpp-objs.dir/connector.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/sockpp-objs.dir/connector.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/connector.cpp.o -MF CMakeFiles/sockpp-objs.dir/connector.cpp.o.d -o CMakeFiles/sockpp-objs.dir/connector.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/connector.cpp"

src/CMakeFiles/sockpp-objs.dir/connector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/connector.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/connector.cpp" > CMakeFiles/sockpp-objs.dir/connector.cpp.i

src/CMakeFiles/sockpp-objs.dir/connector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/connector.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/connector.cpp" -o CMakeFiles/sockpp-objs.dir/connector.cpp.s

src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o: ../src/datagram_socket.cpp
src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o -MF CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o.d -o CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/datagram_socket.cpp"

src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/datagram_socket.cpp" > CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.i

src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/datagram_socket.cpp" -o CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.s

src/CMakeFiles/sockpp-objs.dir/exception.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/exception.cpp.o: ../src/exception.cpp
src/CMakeFiles/sockpp-objs.dir/exception.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/sockpp-objs.dir/exception.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/exception.cpp.o -MF CMakeFiles/sockpp-objs.dir/exception.cpp.o.d -o CMakeFiles/sockpp-objs.dir/exception.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/exception.cpp"

src/CMakeFiles/sockpp-objs.dir/exception.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/exception.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/exception.cpp" > CMakeFiles/sockpp-objs.dir/exception.cpp.i

src/CMakeFiles/sockpp-objs.dir/exception.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/exception.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/exception.cpp" -o CMakeFiles/sockpp-objs.dir/exception.cpp.s

src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.o: ../src/inet_address.cpp
src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.o -MF CMakeFiles/sockpp-objs.dir/inet_address.cpp.o.d -o CMakeFiles/sockpp-objs.dir/inet_address.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/inet_address.cpp"

src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/inet_address.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/inet_address.cpp" > CMakeFiles/sockpp-objs.dir/inet_address.cpp.i

src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/inet_address.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/inet_address.cpp" -o CMakeFiles/sockpp-objs.dir/inet_address.cpp.s

src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o: ../src/inet6_address.cpp
src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o -MF CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o.d -o CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/inet6_address.cpp"

src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/inet6_address.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/inet6_address.cpp" > CMakeFiles/sockpp-objs.dir/inet6_address.cpp.i

src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/inet6_address.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/inet6_address.cpp" -o CMakeFiles/sockpp-objs.dir/inet6_address.cpp.s

src/CMakeFiles/sockpp-objs.dir/socket.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/socket.cpp.o: ../src/socket.cpp
src/CMakeFiles/sockpp-objs.dir/socket.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/sockpp-objs.dir/socket.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/socket.cpp.o -MF CMakeFiles/sockpp-objs.dir/socket.cpp.o.d -o CMakeFiles/sockpp-objs.dir/socket.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/socket.cpp"

src/CMakeFiles/sockpp-objs.dir/socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/socket.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/socket.cpp" > CMakeFiles/sockpp-objs.dir/socket.cpp.i

src/CMakeFiles/sockpp-objs.dir/socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/socket.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/socket.cpp" -o CMakeFiles/sockpp-objs.dir/socket.cpp.s

src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o: ../src/stream_socket.cpp
src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o -MF CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o.d -o CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/stream_socket.cpp"

src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/stream_socket.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/stream_socket.cpp" > CMakeFiles/sockpp-objs.dir/stream_socket.cpp.i

src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/stream_socket.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/stream_socket.cpp" -o CMakeFiles/sockpp-objs.dir/stream_socket.cpp.s

src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o: src/CMakeFiles/sockpp-objs.dir/flags.make
src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o: ../src/unix/unix_address.cpp
src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o: src/CMakeFiles/sockpp-objs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o -MF CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o.d -o CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o -c "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/unix/unix_address.cpp"

src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.i"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/unix/unix_address.cpp" > CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.i

src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.s"
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src/unix/unix_address.cpp" -o CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.s

sockpp-objs: src/CMakeFiles/sockpp-objs.dir/acceptor.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/connector.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/datagram_socket.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/exception.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/inet_address.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/inet6_address.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/socket.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/stream_socket.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/unix/unix_address.cpp.o
sockpp-objs: src/CMakeFiles/sockpp-objs.dir/build.make
.PHONY : sockpp-objs

# Rule to build all files generated by this target.
src/CMakeFiles/sockpp-objs.dir/build: sockpp-objs
.PHONY : src/CMakeFiles/sockpp-objs.dir/build

src/CMakeFiles/sockpp-objs.dir/clean:
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" && $(CMAKE_COMMAND) -P CMakeFiles/sockpp-objs.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/sockpp-objs.dir/clean

src/CMakeFiles/sockpp-objs.dir/depend:
	cd "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp" "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/src" "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug" "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src" "/home/noec/Documents/scuola/ETH/FS_22/Software Engineering/git/blackjack/sockpp/cmake-build-debug/src/CMakeFiles/sockpp-objs.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/sockpp-objs.dir/depend
