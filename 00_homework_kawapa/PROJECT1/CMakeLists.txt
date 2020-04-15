cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project)

set(SRC_LIST
    src/Philosopher.cpp
    src/Philosopher.hpp
    src/Book.cpp
    src/Book.hpp
    src/Reflection.cpp
    src/Reflection.hpp
    src/Wait.cpp
    src/Print.cpp
    src/Calculate.cpp
    src/Write.hpp
    src/Write.cpp

)

add_library(${PROJECT_NAME}-lib ${SRC_LIST})
add_executable(${PROJECT_NAME} src/main.cpp)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib pthread)
endif (UNIX)


set(TEST_SRC_LIST
    tests/CatchMain.cpp
    tests/BookTests.cpp
    tests/ReflectionTests.cpp
    tests/PhilosopherTests.cpp
    tests/WriteTests.cpp
)

set(LIBS Catch2::Catch2 pthread)

add_executable(${PROJECT_NAME}-ut ${TEST_SRC_LIST})
find_package(Catch2 REQUIRED)
target_link_libraries(${PROJECT_NAME}-ut ${PROJECT_NAME}-lib ${LIBS})
target_compile_options(${PROJECT_NAME}-ut PUBLIC -Wall -Werror -Wpedantic -Wextra)
