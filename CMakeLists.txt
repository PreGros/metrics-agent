cmake_minimum_required(VERSION 3.16)
project(metrics_agent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_ASAN "AddressSanitizer" OFF)
option(ENABLE_TSAN "ThreadSanitizer" OFF)

add_compile_options(-Wall -Wextra -Wpedantic)
if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()
if(ENABLE_TSAN)
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
endif()

add_library(metrics src/logger.cpp)
target_include_directories(metrics PUBLIC include)
find_package(Threads REQUIRED)
target_link_libraries(metrics PUBLIC Threads::Threads)

add_executable(metricsd app/main.cpp)
target_link_libraries(metricsd PRIVATE metrics)

# GoogleTest přes FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/test_logger.cpp tests/test_ring_buffer.cpp)
target_link_libraries(tests PRIVATE metrics GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)