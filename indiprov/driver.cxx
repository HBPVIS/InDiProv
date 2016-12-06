#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "controller/activity-actions.hxx"
#include "controller/agent-actions.hxx"
#include "controller/entity-actions.hxx"
#include "controller/wasGeneratedBy-actions.hxx"
#include "controller/wasDerivedFrom-actions.hxx"
#include "controller/wasAttributedTo-actions.hxx"
#include "controller/used-actions.hxx"
#include "controller/wasInformedBy-actions.hxx"

using namespace std;
using namespace odb::core;

int main (int argc, char* argv[]) {
	try {
		auto_ptr<database> db(create_database (argc, argv));

		cout << "create " << createEntity(db, "RWTH") << std::endl;
		cout << "create " << createEntity(db, "VR Lab") << std::endl;

		int actAId = createActivity(db, "Programming", 815, 1630);
		cout << "create Programming Activity " << actAId << std::endl;
		int actBId = createActivity(db, "Hacking", 1400, 1930);
		cout << "create Hacking Activity " << actBId << std::endl;

		cout << "create " << createAgent(db, "cyremur") << std::endl;
		cout << "create " << createAgent(db, "mercury") << std::endl;

		int id = createWasGeneratedBy(db, "VR Lab", actBId);
		cout << "create generation " << id << std::endl;
		cout << "delete generation " << deleteWasGeneratedBy(db, id) << std::endl;

		id = createWasAttributedTo(db, "VR Lab", "cyremur");
		cout << "create attribution " << id << std::endl;
		cout << "delete attribution " << deleteWasAttributedTo(db, id) << std::endl;

		id = createWasDerivedFrom(db, "VR Lab", "RWTH");
		cout << "create derivation " << id << std::endl;
		cout << "delete derivation " << deleteWasDerivedFrom(db, id) << std::endl;

		id = createWasInformedBy(db, actAId, actBId);
		cout << "create information " << id << std::endl;
		cout << "delete information " << deleteWasInformedBy(db, id);

		id = createUsed(db, actAId, "VR Lab");
		cout << "create used " << id << std::endl;
		cout << "delete used " << deleteUsed(db, id) << std::endl;

		cout << "delete cyremur " << deleteAgent(db, "cyremur") << std::endl;
		cout << "delete mercury " << deleteAgent(db, "mercury") << std::endl;
		cout << "delete mercury " << deleteAgent(db, "mercury") << std::endl;

		cout << "delete Programming " << deleteActivity(db, actAId) << std::endl;
		cout << "delete Hacking " << deleteActivity(db, actBId) << std::endl;

		cout << "delete RWTH " << deleteEntity(db, "RWTH") << std::endl;
		cout << "delete VR Lab " << deleteEntity(db, "VR Lab") << std::endl;
	} catch(const odb::exception& e) {
		cerr << e.what () << endl;
		string nope;
		cin >> nope;
		return 1;
	}
	return 0;
}
