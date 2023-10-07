cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER clang++)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error -std=c++17")

include(cmake/Definitions.cmake)

include_directories(/usr/local/include)
include_directories(/usr/include)
include_directories(/usr/src/linux-raspi-headers-5.15.0-1034/include)
include_directories(/usr/local/lib)

project(test)

add_subdirectory(libs/spotify-api-plusplus)
add_subdirectory(src)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/libs)

set(LINK_LIBRARIES
    ssl
    crypto
    network
    spotify-api-plusplus
    display
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})
