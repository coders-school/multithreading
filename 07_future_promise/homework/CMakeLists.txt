cmake_minimum_required(VERSION 3.8)
project(NotificationSender)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#add_library(mail STATIC 3rdparty/libmail/src/send_mail.cxx)

add_library(mail STATIC 3rdparty/libmail2.0/src/send_mail2.0.cxx)
target_include_directories(mail PUBLIC 3rdparty)

add_executable(app notification_sender.cxx)
target_link_libraries(app PRIVATE mail Threads::Threads)
target_compile_options(app PRIVATE -Wall -Werror -Wpedantic -Wextra)
