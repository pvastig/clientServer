cmake_minimum_required(VERSION 3.15)

set(PROJECT client)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(../common)
file(GLOB AUTO_SOURCES "src/*.cpp" "src/*.h" "../common/*.h" "../common/*.cpp")

find_package(Threads REQUIRED)
find_package(Boost 1.70 REQUIRED system log)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_executable(${PROJECT} ${AUTO_SOURCES})
target_link_libraries(${PROJECT} Threads::Threads)
target_link_libraries(${PROJECT} ${Boost_LIBRARIES})
