#define DATABASE_MYSQL

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
#include "controller/wasAssociatedWith-actions.hxx"
#include "controller/actedOnBehalfOf-actions.hxx"
//
//#include <nett/nett.h>
//#include <../schema/string_message.pb.h>

using namespace std;
using namespace odb::core;

int main (int argc, char* argv[]) {
	const std::string endpoint("tcp://127.0.0.1:6555");

	//nett::initialize(endpoint);
	//auto slotOut = nett::make_slot_out<string_message>("string_event");

	//auto slotIn = nett::make_slot_in<string_message>();
	//slotIn->connect(nett::slot_address(endpoint, "string_event"));

	//std::this_thread::sleep_for(std::chrono::seconds(1)); //give time to communicate subscription topics

	//string_message string_message_one;
	//string_message_one.set_value("publish as string_event");

	//slotOut->send(string_message_one);
	//auto msg = slotIn->receive();
	//msg.SerializeToOstream(&std::cout);

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
		cout << "delete information " << deleteWasInformedBy(db, id) << std::endl;

		id = createUsed(db, actAId, "VR Lab");
		cout << "create used " << id << std::endl;
		cout << "delete used " << deleteUsed(db, id) << std::endl;

		id = createWasAssociatedWith(db, actBId, "cyremur");
		cout << "create association " << id << std::endl;
		cout << "delete association " << deleteWasAssociatedWith(db, id) << std::endl;

		id = createActedOnBehalfOf(db, "mercury", "cyremur");
		cout << "create action on behalf " << id << std::endl;
		cout << "delete action on behalf " << deleteActedOnBehalfOf(db, id) << std::endl;

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
	string nope;
	cin >> nope;
	return 0;
}
