cmake_minimum_required(VERSION 3.8)
project(Task2and3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(mail STATIC src/send_mail2.0.cxx)
target_include_directories(mail PUBLIC include/)

add_executable(Task2and3 src/main.cpp)
target_link_libraries(Task2and3 PRIVATE mail Threads::Threads)
target_compile_options(Task2and3 PRIVATE -Wall -Werror -Wpedantic -Wextra) 