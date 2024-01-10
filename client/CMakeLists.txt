cmake_minimum_required(VERSION 3.16)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(pthread CONFIG REQUIRED)

set(CPP_DIR src)
set(CPP ${CPP_DIR}/client.cpp)

include_directories(include)
include_directories(../include/asio-1.28.0/include)
add_executable(${PROJECT_NAME} ${CPP})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)