cmake_minimum_required(VERSION 3.16)

project(DeviceData LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(device_data
    main.cpp
    Analyzer.cpp
)