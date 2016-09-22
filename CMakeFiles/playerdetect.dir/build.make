# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/cesco/pd/intelligentSportMonitoring

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cesco/pd/intelligentSportMonitoring

# Include any dependencies generated for this target.
include CMakeFiles/playerdetect.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/playerdetect.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/playerdetect.dir/flags.make

CMakeFiles/playerdetect.dir/playerdetector.cpp.o: CMakeFiles/playerdetect.dir/flags.make
CMakeFiles/playerdetect.dir/playerdetector.cpp.o: playerdetector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cesco/pd/intelligentSportMonitoring/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/playerdetect.dir/playerdetector.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/playerdetect.dir/playerdetector.cpp.o -c /home/cesco/pd/intelligentSportMonitoring/playerdetector.cpp

CMakeFiles/playerdetect.dir/playerdetector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/playerdetect.dir/playerdetector.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cesco/pd/intelligentSportMonitoring/playerdetector.cpp > CMakeFiles/playerdetect.dir/playerdetector.cpp.i

CMakeFiles/playerdetect.dir/playerdetector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/playerdetect.dir/playerdetector.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cesco/pd/intelligentSportMonitoring/playerdetector.cpp -o CMakeFiles/playerdetect.dir/playerdetector.cpp.s

CMakeFiles/playerdetect.dir/playerdetector.cpp.o.requires:

.PHONY : CMakeFiles/playerdetect.dir/playerdetector.cpp.o.requires

CMakeFiles/playerdetect.dir/playerdetector.cpp.o.provides: CMakeFiles/playerdetect.dir/playerdetector.cpp.o.requires
	$(MAKE) -f CMakeFiles/playerdetect.dir/build.make CMakeFiles/playerdetect.dir/playerdetector.cpp.o.provides.build
.PHONY : CMakeFiles/playerdetect.dir/playerdetector.cpp.o.provides

CMakeFiles/playerdetect.dir/playerdetector.cpp.o.provides.build: CMakeFiles/playerdetect.dir/playerdetector.cpp.o


# Object files for target playerdetect
playerdetect_OBJECTS = \
"CMakeFiles/playerdetect.dir/playerdetector.cpp.o"

# External object files for target playerdetect
playerdetect_EXTERNAL_OBJECTS =

playerdetect: CMakeFiles/playerdetect.dir/playerdetector.cpp.o
playerdetect: CMakeFiles/playerdetect.dir/build.make
playerdetect: /usr/local/lib/libopencv_shape.so.3.1.0
playerdetect: /usr/local/lib/libopencv_stitching.so.3.1.0
playerdetect: /usr/local/lib/libopencv_superres.so.3.1.0
playerdetect: /usr/local/lib/libopencv_videostab.so.3.1.0
playerdetect: /usr/local/lib/libopencv_objdetect.so.3.1.0
playerdetect: /usr/local/lib/libopencv_calib3d.so.3.1.0
playerdetect: /usr/local/lib/libopencv_features2d.so.3.1.0
playerdetect: /usr/local/lib/libopencv_flann.so.3.1.0
playerdetect: /usr/local/lib/libopencv_highgui.so.3.1.0
playerdetect: /usr/local/lib/libopencv_ml.so.3.1.0
playerdetect: /usr/local/lib/libopencv_photo.so.3.1.0
playerdetect: /usr/local/lib/libopencv_video.so.3.1.0
playerdetect: /usr/local/lib/libopencv_videoio.so.3.1.0
playerdetect: /usr/local/lib/libopencv_imgcodecs.so.3.1.0
playerdetect: /usr/local/lib/libopencv_imgproc.so.3.1.0
playerdetect: /usr/local/lib/libopencv_core.so.3.1.0
playerdetect: CMakeFiles/playerdetect.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cesco/pd/intelligentSportMonitoring/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable playerdetect"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/playerdetect.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/playerdetect.dir/build: playerdetect

.PHONY : CMakeFiles/playerdetect.dir/build

CMakeFiles/playerdetect.dir/requires: CMakeFiles/playerdetect.dir/playerdetector.cpp.o.requires

.PHONY : CMakeFiles/playerdetect.dir/requires

CMakeFiles/playerdetect.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/playerdetect.dir/cmake_clean.cmake
.PHONY : CMakeFiles/playerdetect.dir/clean

CMakeFiles/playerdetect.dir/depend:
	cd /home/cesco/pd/intelligentSportMonitoring && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cesco/pd/intelligentSportMonitoring /home/cesco/pd/intelligentSportMonitoring /home/cesco/pd/intelligentSportMonitoring /home/cesco/pd/intelligentSportMonitoring /home/cesco/pd/intelligentSportMonitoring/CMakeFiles/playerdetect.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/playerdetect.dir/depend

