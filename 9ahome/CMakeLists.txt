set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.10)

project(project)
find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )

add_executable(${PROJECT_NAME} main.cpp)
if(UNIX)
    target_link_libraries(${PROJECT_NAME} pthread ${OpenCV_LIBS})
endif(UNIX)