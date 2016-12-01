#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "controller/activity-actions.hxx"
#include "controller/agent-actions.hxx"
#include "controller/entity-actions.hxx"

using namespace std;
using namespace odb::core;

int main (int argc, char* argv[]) {
	try {
		auto_ptr<database> db(create_database (argc, argv));
		cout << "create " << createAgent(db, "cyremur") << std::endl;
		cout << "create " << createAgent(db, "mercury") << std::endl;
		cout << "delete cyremur " << deleteAgent(db, "cyremur") << std::endl;
		cout << "delete mercury " << deleteAgent(db, "mercury") << std::endl;
		cout << "delete mercury " << deleteAgent(db, "mercury") << std::endl;

		cout << "create " << createEntity(db, "RWTH") << std::endl;
		cout << "create " << createEntity(db, "VR Lab") << std::endl;
		cout << "delete RWTH " << deleteEntity(db, "RWTH") << std::endl;
		cout << "delete VR Lab " << deleteEntity(db, "VR Lab") << std::endl;
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
