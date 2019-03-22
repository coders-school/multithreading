cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-but-set-variable)
add_definitions(-Wall -Werror -Wpedantic -Wextra)

add_executable(01_threads_write 01_threads_write.cpp)
target_link_libraries(01_threads_write pthread)

add_executable(02_threads_in_collection 02_threads_in_collection.cpp)
target_link_libraries(02_threads_in_collection pthread)

add_executable(03_shared_mutex 03_shared_mutex.cpp)
target_link_libraries(03_shared_mutex pthread)

add_executable(04_deadlock 04_deadlock.cpp)
target_link_libraries(04_deadlock pthread)

