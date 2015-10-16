cmake_minimum_required(VERSION 2.8)
project(SNMP_Manager)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(HEADER_FILES
    include/params.hxx
    include/packet.hxx
    include/bitmap.hxx
    include/pdu.hxx
    include/pdu_bindings.hxx
    include/manager.hxx
    include/log.hxx)

set(SOURCE_FILES
    source/main.cxx
    source/params.cxx
    source/packet.cxx
    source/manager.cxx
    source/log.cxx)

add_executable(snmp ${SOURCE_FILES} ${HEADER_FILES})

install(PROGRAMS snmp DESTINATION bin)
