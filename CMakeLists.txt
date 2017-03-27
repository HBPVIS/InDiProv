cmake_minimum_required( VERSION 2.8 )

project (InDiProvCMake)

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake )

set ( LIBODB_INCLUDE_PATH "" CACHE PATH "The odb main include path" )
set ( LIBODBDB_INCLUDE_PATH "" CACHE PATH "The odb db implementation include path" )
set ( LIBODB_LIBRARY "" CACHE FILEPATH "The odb library to link against" )
set ( LIBODBDB_LIBRARY "" CACHE FILEPATH "The odb db library to link against" )

set ( DB_CONNECTOR_INCLUDE_PATH "" CACHE PATH "The db connector include path" )
set ( DB_CONNECTOR_LIBRARY "" CACHE FILEPATH "The db connector library to link against" )
set ( DB_USER "root" CACHE STRING "DB user name")
set ( DB_PASS "password" CACHE STRING "DB password")

set ( NETT_INCLUDE_DIR "" CACHE PATH "The nett include path" )
set ( NETT_LIBRARY "" CACHE FILEPATH "The nett library to link against" )
set ( NETT_LIBRARY_DEBUG "" CACHE FILEPATH "The nett debug library to link against" )

if( MSVC )
  set ( ZMQ_INCLUDE_DIR "" CACHE PATH "Path to the include directory of zmq" )
  set ( ZMQ_LIBRARY "" CACHE FILEPATH "The zmq library to link against" )

  set ( PROTOBUF_INCLUDE_DIR "" CACHE PATH "The protobuf include directory" )
  set ( PROTOBUF_PROTOC_EXECUTABLE "" CACHE FILEPATH "The protobuf binary directory for the schema compiler 'protoc'" )
  set ( PROTOBUF_LIBRARY "" CACHE FILEPATH "The protobuf library to link against" )
  set ( PROTOBUF_LIBRARY_DEBUG "" CACHE FILEPATH "The protobuf library to link against" )
elseif( UNIX )
  set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC" )
  include( Findlibzmq )
  find_package( Protobuf REQUIRED )
  find_package( libzmq REQUIRED )
endif()

set ( MODEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/indiprov/Model CACHE INTERNAL "model directory")

file(GLOB models
	"${MODEL_DIR}/vertex.hxx"
	"${MODEL_DIR}/edge.hxx"
)

message("starting odb compiler...")
foreach(model ${models})
	message("  compiling ${model}")
	exec_program("odb" ${MODEL_DIR} ARGS "-d mysql" "--generate-query" "--generate-schema" "--fkeys-deferrable-mode not_deferrable" ${model} OUTPUT_VARIABLE output)
endforeach()
message("odb compilation finished.")
message("")

exec_program("mysql" ARGS "--user=${DB_USER}" "--password=${DB_PASS} -e" "\"DROP DATABASE provenance\"" OUTPUT_VARIABLE output)
exec_program("mysql" ARGS "--user=${DB_USER}" "--password=${DB_PASS}" "-e \"CREATE DATABASE provenance\"" OUTPUT_VARIABLE output)

message("  creating elementary tables...")
message("    mysql --user=${DB_USER} --password=${DB_PASS} --database=provenance < ${query}")
execute_process(COMMAND "mysql" "--user=${DB_USER}" "--password=${DB_PASS}" "--database=provenance" INPUT_FILE "${MODEL_DIR}/vertex.sql" OUTPUT_VARIABLE output)

message("  creating cross tables...")
message("    mysql --user=${DB_USER} --password=${DB_PASS} --database=provenance < ${query}")
execute_process(COMMAND "mysql" "--user=${DB_USER}" "--password=${DB_PASS}" "--database=provenance" INPUT_FILE "${MODEL_DIR}/edge.sql" OUTPUT_VARIABLE output)
message("mysql db setup complete.")

file(GLOB source
	"indiprov/database.cxx"
	"indiprov/Model/*.hxx"
	"indiprov/Model/*.cxx"
	"indiprov/Model/*.ixx"
	"indiprov/Controller/*.hxx"
)

macro (add_event event_name)
  exec_program("${PROTOBUF_PROTOC_EXECUTABLE}" ARGS "${MODEL_DIR}/${event_name}.proto" "--proto_path=${MODEL_DIR}" "--cpp_out=${MODEL_DIR}" OUTPUT_VARIABLE output )
  if (NOT EXISTS "${MODEL_DIR}/${event_name}.pb.h" OR NOT EXISTS "${MODEL_DIR}/${event_name}.pb.cc" )
		message("  ${output}")
    message("  protoc error. files not generated.")
  endif ()
  list (APPEND source "${MODEL_DIR}/${event_name}.pb.h" "${MODEL_DIR}/${event_name}.pb.cc")
  message("  ${event_name} added to source")
endmacro()

message("starting protoc...")
add_event ( creation_messages )
message("protoc compilation complete.")

include_directories (${LIBODB_INCLUDE_PATH})
include_directories (${LIBODBDB_INCLUDE_PATH})
include_directories (${DB_CONNECTOR_INCLUDE_PATH})
include_directories (${NETT_INCLUDE_DIR})
include_directories (${ZMQ_INCLUDE_DIR})
include_directories (${PROTOBUF_INCLUDE_DIR})
include_directories (${MODEL_DIR})

macro (add_target_and_link target)
	add_executable( ${target} "indiprov/${target}.cxx" ${source})
	target_link_libraries( ${target} ${LIBODB_LIBRARY} )
	target_link_libraries( ${target} ${LIBODBDB_LIBRARY} )
	target_link_libraries( ${target} ${DB_CONNECTOR_LIBRARY} )
	target_link_libraries( ${target} optimized ${NETT_LIBRARY} debug ${NETT_LIBRARY_DEBUG})
	target_link_libraries( ${target} ${ZMQ_LIBRARY} )
	target_link_libraries( ${target} optimized ${PROTOBUF_LIBRARY} debug ${PROTOBUF_LIBRARY_DEBUG})
endmacro()

add_target_and_link(server)
add_target_and_link(client)
add_target_and_link(tests)
