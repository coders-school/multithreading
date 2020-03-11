cmake_minimum_required(VERSION 2.8)
project( PatternMatch )
find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )
add_executable( PatternMatch pattern_match.cpp )
target_link_libraries( PatternMatch ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})


# https://docs.opencv.org/2.4/opencv_tutorials.pdf
