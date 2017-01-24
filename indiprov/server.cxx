#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <iostream>
#include <thread>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "controller/vertex-actions.hxx"
#include "controller/edge-actions.hxx"

#include <nett/nett.h>
#include "model/creation_messages.pb.h"

using namespace std;
using namespace odb::core;

const std::string endpoint("tcp://127.0.0.1:6555");


template <typename T>
void createPROV(auto_ptr<database>& db, shared_ptr<nett::slot_in<T>>) {
	return;
}

template <typename T>
void receiveLoop(auto_ptr<database>& db, shared_ptr<nett::slot_in<T>> slotIn) {
}



int main (int argc, char* argv[]) {

	nett::initialize(endpoint);
	try {
		auto_ptr<database> db(create_database (argc, argv));
	} catch(const odb::exception& e) {
		cerr << e.what () << endl;
		string nope;
		cin >> nope;
		return 1;
	}
	string nope;
	cin >> nope;
	return 0;
}
