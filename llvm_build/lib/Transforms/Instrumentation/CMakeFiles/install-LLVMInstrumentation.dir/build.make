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

# Utility rule file for install-LLVMInstrumentation.

# Include any custom commands dependencies for this target.
include lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/progress.make

lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation:
	cd /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/Instrumentation && /usr/bin/cmake -DCMAKE_INSTALL_COMPONENT="LLVMInstrumentation" -P /home/jason/Documents/advanced_compiler/llvm_build/cmake_install.cmake

install-LLVMInstrumentation: lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation
install-LLVMInstrumentation: lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/build.make
.PHONY : install-LLVMInstrumentation

# Rule to build all files generated by this target.
lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/build: install-LLVMInstrumentation
.PHONY : lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/build

lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/clean:
	cd /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/Instrumentation && $(CMAKE_COMMAND) -P CMakeFiles/install-LLVMInstrumentation.dir/cmake_clean.cmake
.PHONY : lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/clean

lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/depend:
	cd /home/jason/Documents/advanced_compiler/llvm_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jason/Documents/advanced_compiler/llvm-project-17.0.2.src/llvm /home/jason/Documents/advanced_compiler/llvm-project-17.0.2.src/llvm/lib/Transforms/Instrumentation /home/jason/Documents/advanced_compiler/llvm_build /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/Instrumentation /home/jason/Documents/advanced_compiler/llvm_build/lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/Transforms/Instrumentation/CMakeFiles/install-LLVMInstrumentation.dir/depend

