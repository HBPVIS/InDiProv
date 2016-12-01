#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "model/agent.hxx"
#include "model/agent-odb.hxx"
#include "dbactions.hxx"

using namespace std;
using namespace odb::core;

int main (int argc, char* argv[]) {
	try {
		auto_ptr<database> db(create_database (argc, argv));
		cout << createAgent(db, "cyremur") << std::endl;
		cout << createAgent(db, "mercury") << std::endl;
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
