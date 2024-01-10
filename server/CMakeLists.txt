cmake_minimum_required(VERSION 3.16)
project(serverAsync)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(glfw3 3.3 CONFIG REQUIRED)

#set(glad_DIR vcpkg_installed/x64-linux/share/glad)
#find_package(glad CONFIG REQUIRED)

#set(glm_DIR vcpkg_installed/x64-linux/share/glm)
#find_package(glm CONFIG REQUIRED)

#find_package(fmt CONFIG REQUIRED)

set(CPP_DIR src)
set(CPP ${CPP_DIR}/serverAsync.cpp)

#include_directories(include)
include_directories(../lib/asio/asio/include)
#include_directories(vcpkg_installed/x64-linux/include)
add_executable(${PROJECT_NAME} ${CPP})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
#target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
#target_include_directories(${PROJECT_NAME} PRIVATE vcpkg_installed/x64-linux/include)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)