project(playerdetector)

cmake_minimum_required (VERSION 2.8 )
cmake_policy(SET CMP0015 NEW)
set(CMAKE_CXX_FLAGS "-std=c++11")
set (OpenCV_DIR /home/cesco/opencv-2.4.13)
find_package( OpenCV REQUIRED )



add_executable(playerdetect playerdetector.cpp)
target_link_libraries( playerdetect ${OpenCV_LIBS})

add_executable(aux aux.cpp)
target_link_libraries( aux ${OpenCV_LIBS})
