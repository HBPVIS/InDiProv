#define DATABASE_MYSQL

//pistache includes
#include "pistache/http.h"
#include "pistache/description.h"
#include "pistache/endpoint.h"
#include "rapidjson/rapidjson.h"


#include <memory>   // std::auto_ptr
#include <iostream>
#include <thread>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "Controller/vertex-actions.hxx"
#include "Controller/edge-actions.hxx"

using namespace odb::core;

std::auto_ptr<database> db;

int main (int argc, char* argv[]) {
	std::cout << "Initializing server..." << std::endl;
	try {
		db = create_database(argc, argv);
	} catch(const odb::exception& e) {
		std::cerr << e.what () << std::endl;
		return 1;
	}

	Net::Port port(9080);
	int thr = 2;

	Net::Address addr(Net::Ipv4::any(), port);

	std::cout << "Cores = " << hardware_concurrency() << std::endl;
	std::cout << "Using " << thr << " threads" << std::endl;

	std::cout << "Server startup complete." << std::endl;

	return 0;
}
