#define DATABASE_MYSQL

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
	std::cout << "Server startup complete." << std::endl;

	return 0;
}