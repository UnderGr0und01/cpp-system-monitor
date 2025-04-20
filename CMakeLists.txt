cmake_minimum_required(VERSION 3.10)
project(SystemMonitor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include third_party/inih)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCES
    src/collectors/cpu_collector.cpp
    src/collectors/memory_collector.cpp
    src/collectors/network_collector.cpp
    src/data_aggregator.cpp
    src/ncurses_output.cpp
    src/config.cpp
    src/main.cpp
    third_party/inih/ini.c
)
add_executable(monitor ${SOURCES})
target_link_libraries(monitor PRIVATE pthread ${CURSES_LIBRARIES})

message("Sources: ${SOURCES}")
message("Curses libraries: ${CURSES_LIBRARIES}")