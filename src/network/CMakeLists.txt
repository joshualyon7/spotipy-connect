set(SOURCES
    TcpServer.cpp)

set(HEADERS httplib.h TcpServer.h)

add_library(network STATIC ${SOURCES} ${HEADERS})

target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
