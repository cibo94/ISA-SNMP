cmake_minimum_required(VERSION 3.2)
project(SNMP_Manager)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -g")

set(HEADER_FILES
    include/params.hxx
    include/packet.hxx
    include/bitmap.hxx
    include/pdu.hxx
    include/pdu_bindings.hxx
    include/manager.hxx)

set(SOURCE_FILES
    source/main.cxx
    source/params.cxx
    source/packet.cxx
    source/manager.cxx)

add_executable(snmp ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(snmp ${CMAKE_THREAD_LIBS_INIT})

install(PROGRAMS snmp DESTINATION bin)
