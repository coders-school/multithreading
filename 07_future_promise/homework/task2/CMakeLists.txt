cmake_minimum_required(VERSION 3.8)
project(NotificationSender)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(mail STATIC 3rdparty/libmail/src/send_mail.cpp)
target_include_directories(mail PUBLIC 3rdparty/libmail/include)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE mail Threads::Threads)
target_compile_options(app PRIVATE -Wall -Werror -Wpedantic -Wextra)

add_executable(solution solution/main.cpp)
target_link_libraries(solution PRIVATE mail Threads::Threads)
target_compile_options(solution PRIVATE -Wall -Werror -Wpedantic -Wextra)