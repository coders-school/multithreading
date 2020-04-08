cmake_minimum_required(VERSION 3.8)
project(Task1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(mail STATIC src/send_mail.cxx)
target_include_directories(mail PUBLIC include/)

add_executable(Task1 src/main.cpp)
target_link_libraries(Task1 PRIVATE mail Threads::Threads)
target_compile_options(Task1 PRIVATE -Wall -Werror -Wpedantic -Wextra) 