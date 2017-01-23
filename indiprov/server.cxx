#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <iostream>
#include <thread>

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

#include <nett/nett.h>
#include "model/creation_messages.pb.h"

using namespace std;
using namespace odb::core;

const std::string endpoint("tcp://127.0.0.1:6555");

shared_ptr<nett::slot_in <agent_creation>> agentSlotIn;
shared_ptr<nett::slot_in <entity_creation>> entitySlotIn;
shared_ptr<nett::slot_in <activity_creation>> activitySlotIn;
shared_ptr<thread> tAgent;
shared_ptr<thread> tEntity;
shared_ptr<thread> tActivity;


template <typename T>
void createPROV(auto_ptr<database>& db, shared_ptr<nett::slot_in<T>>) {
	return;
}

template <typename T>
void receiveLoop(auto_ptr<database>& db, shared_ptr<nett::slot_in<T>> slotIn) {
	//slotIn = nett::make_slot_in<T>();
	//slotIn->connect(nett::slot_address(endpoint, "creation"));
	while (true) {
		cout << "loop" << endl;
		auto msg = slotIn->receive();
		auto name = msg.name();
		cout << name << std::endl;
		cout << "create " << createPROV(db, msg) << std::endl;
	}
}



int main (int argc, char* argv[]) {

	nett::initialize(endpoint);
	agentSlotIn = nett::make_slot_in<agent_creation>();
	agentSlotIn->connect(nett::slot_address(endpoint, "agent_creation"));
	entitySlotIn = nett::make_slot_in<entity_creation>();
	entitySlotIn->connect(nett::slot_address(endpoint, "entity_creation"));
	activitySlotIn = nett::make_slot_in<activity_creation>();
	activitySlotIn->connect(nett::slot_address(endpoint, "activity_creation"));
	try {
		auto_ptr<database> db(create_database (argc, argv));
		cout << "test" << endl;
		tAgent = make_shared<thread>(&receiveLoop<agent_creation>, db, agentSlotIn);
		tEntity = make_shared<thread>(&receiveLoop<entity_creation>, db, entitySlotIn);
		tActivity = make_shared<thread>(&receiveLoop<activity_creation>, db, activitySlotIn);
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
