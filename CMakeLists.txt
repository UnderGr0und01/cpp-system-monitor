# cmake_minimum_required(VERSION 3.10)
# project(SystemMonitor)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(include)
# file(GLOB SOURCES "src/*.cpp")
# add_executable(monitor ${SOURCES})
# target_link_libraries(monitor PRIVATE pthread)

cmake_minimum_required(VERSION 3.10)
project(SystemMonitor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
set(SOURCES
    src/cpu_collector.cpp
    src/data_aggregator.cpp
    src/console_output.cpp
    src/main.cpp
    src/memory_collector.cpp
    src/network_collector.cpp
)
add_executable(monitor ${SOURCES})
target_link_libraries(monitor PRIVATE pthread)