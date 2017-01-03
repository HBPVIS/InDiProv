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
set ( MODEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/indiprov/model )

message(${MODEL_DIR})

set ( ZMQ_INCLUDE_DIR "" CACHE PATH "Path to the include directory of zmq" )
set ( ZMQ_LIBRARY "" CACHE FILEPATH "The zmq library to link against" )

set ( PROTOBUF_INCLUDE_DIR "" CACHE PATH "The protobuf include directory" )
set ( PROTOBUF_BIN "" CACHE PATH "The protobuf binary directory for the schema compiler 'protoc'" )
set ( PROTOBUF_LIBRARY "" CACHE FILEPATH "The protobuf library to link against" )
set ( PROTOBUF_LIBRARY_DEBUG "" CACHE FILEPATH "The protobuf library to link against" )


file(GLOB models
	"${MODEL_DIR}/*.hxx"
)

message("compile odb")
foreach(model ${models})
	exec_program("odb" ${MODEL_DIR} ARGS "-d mysql" "--generate-query" "--generate-schema" "--fkeys-deferrable-mode not_deferrable" ${model} OUTPUT_VARIABLE output)
endforeach()

file(GLOB querys
	"${MODEL_DIR}/*.sql"
)
exec_program("mysql" ARGS "--user=root" "--password=password -e" "\"DROP DATABASE provenance\"" OUTPUT_VARIABLE output)
message ("${output}")
exec_program("mysql" ARGS "--user=root" "--password=password" "-e \"CREATE DATABASE provenance\"" OUTPUT_VARIABLE output)
message ("${output}")
foreach(query ${querys})
	message("mysql --user=root --password=password --database=provenance < ${query}")
	execute_process(COMMAND "mysql" "--user=root" "--password=password" "--database=provenance" INPUT_FILE "${query}" OUTPUT_VARIABLE output)
endforeach()

file(GLOB source
	"indiprov/*.hxx"
	"indiprov/*.cxx"
	"indiprov/Model/*.hxx"
	"indiprov/Model/*.cxx"
	"indiprov/Model/*.ixx"
	"indiprov/Controller/*.hxx"
)

macro (add_event event_name)
	message("starting protoc")
  exec_program( "./protoc" "${PROTOBUF_BIN}" ARGS "${MODEL_DIR}/${event_name}.proto" "--proto_path=${MODEL_DIR}" "--cpp_out=${MODEL_DIR}" OUTPUT_VARIABLE output )
  message("${output}")
  
  if ( NOT EXISTS "${MODEL_DIR}/${event_name}.pb.h" OR NOT EXISTS "${MODEL_DIR}/${event_name}.pb.cc" )
    message("protoc error. files not generated.")
  endif ()
  list (APPEND source "${MODEL_DIR}/${event_name}.pb.h" "${MODEL_DIR}/${event_name}.pb.cc")
  message("${event_name} added to source")
endmacro()
add_event ( creation_messages )


include_directories (${LIBODB_INCLUDE_PATH})
include_directories (${LIBODB_DB_INCLUDE_PATH})
include_directories (${DB_CONNECTOR_INCLUDE_PATH})
include_directories (${NETT_INCLUDE_DIR})
include_directories (${ZMQ_INCLUDE_DIR})
include_directories (${PROTOBUF_INCLUDE_DIR})
include_directories (${MODEL_DIR})

add_executable( driver ${ProjectSources} ${source})
target_link_libraries( driver ${LIBODB_LIBRARY} )
target_link_libraries( driver ${LIBODB_DB_LIBRARY} )
target_link_libraries( driver ${DB_CONNECTOR_LIBRARY} )
target_link_libraries( driver optimized ${NETT_LIBRARY} debug ${NETT_LIBRARY_DEBUG})
target_link_libraries( driver ${ZMQ_LIBRARY} )
target_link_libraries( driver optimized ${PROTOBUF_LIBRARY} debug ${PROTOBUF_LIBRARY_DEBUG})