set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.10)

project(hw4_task9)
find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )

add_executable(hw4_task9a hw4_task9a.cpp)
target_link_libraries(hw4_task9a pthread ${OpenCV_LIBS})
target_compile_options(hw4_task9a PRIVATE -Wall -Werror -Wpedantic -Wextra)
