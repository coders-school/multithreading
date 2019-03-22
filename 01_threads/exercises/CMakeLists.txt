cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-but-set-variable)
add_definitions(-Wall -Werror -Wpedantic -Wextra)

add_executable(01_params_passing 01_params_passing.cpp)
target_link_libraries(01_params_passing pthread)

add_executable(02_reference_passing 02_reference_passing.cpp)
target_link_libraries(02_reference_passing pthread)

add_executable(03_methods_passing 03_methods_passing.cpp)
target_link_libraries(03_methods_passing pthread)

add_executable(04_scoped_thread 04_scoped_thread.cpp)
target_link_libraries(04_scoped_thread pthread)

add_executable(05_threads_in_collection 05_threads_in_collection.cpp)
target_link_libraries(05_threads_in_collection pthread)

