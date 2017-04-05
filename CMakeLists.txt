cmake_minimum_required( VERSION 2.8 )

project (InDiProvCMake)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC" )

set ( LIBODB_INCLUDE_PATH "" CACHE PATH "The odb main include path" )
set ( LIBODBDB_INCLUDE_PATH "" CACHE PATH "The odb db implementation include path" )
set ( LIBODB_LIBRARY "" CACHE FILEPATH "The odb library to link against" )
set ( LIBODBDB_LIBRARY "" CACHE FILEPATH "The odb db library to link against" )

set ( DB_CONNECTOR_INCLUDE_PATH "" CACHE PATH "The db connector include path" )
set ( DB_CONNECTOR_LIBRARY "" CACHE FILEPATH "The db connector library to link against" )
set ( DB_USER "root" CACHE STRING "DB user name")
set ( DB_PASS "password" CACHE STRING "DB password")

set ( PISTACHE_INCLUDE_PATH "" CACHE PATH "Pistache REST Server includes" )

find_package(RapidJSON)
find_package( Threads REQUIRED )

set ( MODEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Model CACHE INTERNAL "model directory")

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
	"src/database.cxx"
	"src/Model/*.hxx"
	"src/Model/*.cxx"
	"src/Model/*.ixx"
	"src/Controller/*.hxx"
)

include_directories (${LIBODB_INCLUDE_PATH})
include_directories (${LIBODBDB_INCLUDE_PATH})
include_directories (${DB_CONNECTOR_INCLUDE_PATH})
include_directories (${MODEL_DIR})
include_directories (${PISTACHE_INCLUDE_PATH})
include_directories (${RapidJSON_INCLUDE_DIR})

add_executable( server "src/server.cxx" ${source} )
target_link_libraries( server ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( server ${LIBODB_LIBRARY} )
target_link_libraries( server ${LIBODBDB_LIBRARY} )
target_link_libraries( server ${DB_CONNECTOR_LIBRARY} )
target_link_libraries( server net_static)

add_executable( client "src/client.cxx")
