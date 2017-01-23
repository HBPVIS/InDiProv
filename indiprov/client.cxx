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

//shared_ptr<nett::slot_in <agent_creation>> agentSlotIn;
//shared_ptr<nett::slot_in <entity_creation>> entitySlotIn;
//shared_ptr<nett::slot_in <activity_creation>> activitySlotIn;
//shared_ptr<thread> tAgent;
//shared_ptr<thread> tEntity;
//shared_ptr<thread> tActivity;


int main(int argc, char* argv[]) {

	nett::initialize(endpoint);
	auto slotOut = nett::make_slot_out<agent_creation>("agent_creation");

	string nope;
	cin >> nope;
	return 0;
}
