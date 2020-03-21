cmake_minimum_required(VERSION 2.8)
project( PatternMatch )
find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )
add_executable( PatternMatch pattern_match.cpp )
add_executable( LineMatch line_match.cpp )
add_executable( LineMatch2 line_match2.cpp )
add_executable( TestOCV test_ocv.cpp )
target_link_libraries( PatternMatch ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries( LineMatch ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries( LineMatch2 ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries( TestOCV ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

# https://docs.opencv.org/2.4/opencv_tutorials.pdf
