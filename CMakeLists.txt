cmake_minimum_required( VERSION 2.8 )

project (InDiProvCMake)

set ( LIBODB_INCLUDE_PATH "" CACHE PATH "The odb main include path" )
set ( LIBODB_DB_INCLUDE_PATH "" CACHE PATH "The odb db implementation include path" )
set ( LIBODB_LIBRARY "" CACHE FILEPATH "The odb library to link against" )
set ( LIBODB_DB_LIBRARY "" CACHE FILEPATH "The odb db library to link against" )

set ( DB_CONNECTOR_INCLUDE_PATH "" CACHE PATH "The db connector include path" )
set ( DB_CONNECTOR_LIBRARY "" CACHE FILEPATH "The db connector library to link against" )

set ( NETT_INCLUDE_DIR "" CACHE PATH "The nett include path" )
set ( NETT_LIBRARY "" CACHE FILEPATH "The nett library to link against" )
set ( NETT_LIBRARY_DEBUG "" CACHE FILEPATH "The nett debug library to link against" )
set ( NETT_SCHEMA_DIR "" CACHE PATH "The nett schema path" )

set ( ZMQ_INCLUDE_DIR "" CACHE PATH "Path to the include directory of zmq" )
set ( ZMQ_LIBRARY "" CACHE FILEPATH "The zmq library to link against" )

set ( PROTOBUF_INCLUDE_DIR "" CACHE PATH "The protobuf include directory" )
set ( PROTOBUF_BIN "" CACHE PATH "The protobuf binary directory for the schema compiler 'protoc'" )
set ( PROTOBUF_LIBRARY "" CACHE FILEPATH "The protobuf library to link against" )
set ( PROTOBUF_LIBRARY_DEBUG "" CACHE FILEPATH "The protobuf library to link against" )

macro (add_event arg1 event_name)
  exec_program( "./protoc" "${PROTOBUF_BIN}" ARGS "${NETT_SCHEMA_DIR}/${event_name}.proto" "--proto_path=${NETT_SCHEMA_DIR}" "--cpp_out=${NETT_SCHEMA_DIR}" OUTPUT_VARIABLE output )
  message ("output of protoc is: ${output}")
  message ("${NETT_SCHEMA_DIR}")
  
  if ( NOT EXISTS "${NETT_SCHEMA_DIR}/${event_name}.pb.h" OR NOT EXISTS "${NETT_SCHEMA_DIR}/${event_name}.pb.cc" )
    message("files not generated")
  endif ()
  list (APPEND source "${NETT_SCHEMA_DIR}/${event_name}.pb.h" "${NETT_SCHEMA_DIR}/${event_name}.pb.cc")
  message("${event_name} added to source")
endmacro()

file(GLOB source
	"indiprov/*.hxx"
	"indiprov/*.cxx"
	"indiprov/Model/*.hxx"
	"indiprov/Model/*.cxx"
	"indiprov/Model/*.ixx"
	"indiprov/Controller/*.hxx"
)

include_directories (${LIBODB_INCLUDE_PATH})
include_directories (${LIBODB_DB_INCLUDE_PATH})
include_directories (${DB_CONNECTOR_INCLUDE_PATH})
include_directories (${NETT_INCLUDE_DIR})
include_directories (${ZMQ_INCLUDE_DIR})
include_directories (${PROTOBUF_INCLUDE_DIR})
include_directories (${NETT_SCHEMA_DIR})

add_event ( driver creation_messages )

add_executable( driver ${ProjectSources} ${source})
target_link_libraries( driver ${LIBODB_LIBRARY} )
target_link_libraries( driver ${LIBODB_DB_LIBRARY} )
target_link_libraries( driver ${DB_CONNECTOR_LIBRARY} )
target_link_libraries( driver optimized ${NETT_LIBRARY} debug ${NETT_LIBRARY_DEBUG})
target_link_libraries( driver ${ZMQ_LIBRARY} )
target_link_libraries( driver optimized ${PROTOBUF_LIBRARY} debug ${PROTOBUF_LIBRARY_DEBUG})