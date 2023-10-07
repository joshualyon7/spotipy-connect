set(SOURCES
    Display.cpp)

set(HEADERS Display.h)

add_library(display STATIC ${SOURCES} ${HEADERS})

target_include_directories(display PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(LINK_LIBRARIES
    spotify-api-plusplus)

if (NOT BUILD_WSL)
    list(APPEND LINK_LIBRARIES i2c liquidcrystali2c)
    target_compile_definitions(display PUBLIC BUILD_PI)
endif()

target_link_libraries(display ${LINK_LIBRARIES})