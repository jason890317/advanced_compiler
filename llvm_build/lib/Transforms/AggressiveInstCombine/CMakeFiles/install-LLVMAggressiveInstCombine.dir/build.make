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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jason/Documents/advanced_compiler/llvm-project-17.0.2.src/llvm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jason/Documents/advanced_compiler/llvm_build

# Utility rule file for install-LLVMAggressiveInstCombine.

# Include any custom commands dependencies for this target.
include lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/progress.make

lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine:
	cd /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/AggressiveInstCombine && /usr/bin/cmake -DCMAKE_INSTALL_COMPONENT="LLVMAggressiveInstCombine" -P /home/jason/Documents/advanced_compiler/llvm_build/cmake_install.cmake

install-LLVMAggressiveInstCombine: lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine
install-LLVMAggressiveInstCombine: lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/build.make
.PHONY : install-LLVMAggressiveInstCombine

# Rule to build all files generated by this target.
lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/build: install-LLVMAggressiveInstCombine
.PHONY : lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/build

lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/clean:
	cd /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/AggressiveInstCombine && $(CMAKE_COMMAND) -P CMakeFiles/install-LLVMAggressiveInstCombine.dir/cmake_clean.cmake
.PHONY : lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/clean

lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/depend:
	cd /home/jason/Documents/advanced_compiler/llvm_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jason/Documents/advanced_compiler/llvm-project-17.0.2.src/llvm /home/jason/Documents/advanced_compiler/llvm-project-17.0.2.src/llvm/lib/Transforms/AggressiveInstCombine /home/jason/Documents/advanced_compiler/llvm_build /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/AggressiveInstCombine /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/Transforms/AggressiveInstCombine/CMakeFiles/install-LLVMAggressiveInstCombine.dir/depend

