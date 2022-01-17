cmake_minimum_required(VERSION 3.11.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main # release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-parameter)
add_definitions(-Wall -Werror -pedantic -Wextra)

file(GLOB SOURCES module*/e*/*.cpp)

foreach(FILE ${SOURCES})
    get_filename_component(BINNAME ${FILE} NAME_WE)
    add_executable(${BINNAME} ${FILE})
    target_link_libraries(${BINNAME} pthread -fsanitize=thread)
endforeach(FILE)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)
