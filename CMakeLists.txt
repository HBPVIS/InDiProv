cmake_minimum_required( VERSION 2.8 )

set (NETT_INCLUDE_DIR "" CACHE PATH "The nett include path" )
set (NETT_LIBRARY "" CACHE FILEPATH "The nett library to link against" )
set (NETT_LIBRARY_DEBUG "" CACHE FILEPATH "The nett debug library to link against" )
set (NETT_CMAKE_HELPERS "" CACHE PATH "The nett library to link against" )
set ( ZMQ_INCLUDE_DIR "" CACHE PATH "Path to the include directory of zmq" )
set ( ZMQ_LIBRARY "" CACHE FILEPATH "The zmq library to link against" )
set ( PROTOBUF_INCLUDE_DIR "" CACHE PATH "The protobuf include directory" )

set( PROTOBUF_BIN "" CACHE PATH "The protobuf binary directory for the schema compiler 'protoc'" )
set ( PROTOBUF_LIBRARY "" CACHE FILEPATH "The protobuf library to link against" )
set ( PROTOBUF_LIBRARY_DEBUG "" CACHE FILEPATH "The protobuf library to link against" )
set ( NETT_SCHEMA_DIR "" CACHE PATH "The nett schema path" )

include( "${NETT_CMAKE_HELPERS}/CMakeHelpers.cmake" )

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/Source")
include_directories (${NETT_INCLUDE_DIR})
include_directories (${ZMQ_INCLUDE_DIR})
include_directories (${PROTOBUF_INCLUDE_DIR})
include_directories (${NETT_SCHEMA_DIR})

include( "Source/_SourceFiles.cmake" )
add_event ( Server float_message ${NETT_SCHEMA_DIR} "")
add_event ( Server string_message ${NETT_SCHEMA_DIR} "")

add_executable( Server ${ProjectSources} ${Server_EVENT_SOURCE_FILES})
target_link_libraries( Server optimized ${NETT_LIBRARY} debug ${NETT_LIBRARY_DEBUG})
target_link_libraries( Server optimized ${PROTOBUF_LIBRARY} debug ${PROTOBUF_LIBRARY_DEBUG})
target_link_libraries( Server ${ZMQ_LIBRARY} )

