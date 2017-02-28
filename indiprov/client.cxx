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

//shared_ptr<nett::slot_in <agent_creation>> agentSlotIn;
//shared_ptr<nett::slot_in <entity_creation>> entitySlotIn;
//shared_ptr<nett::slot_in <activity_creation>> activitySlotIn;
//shared_ptr<thread> tAgent;
//shared_ptr<thread> tEntity;
//shared_ptr<thread> tActivity;


int main(int argc, char* argv[]) {

	nett::initialize(endpoint);

	string nope;
	cin >> nope;
	return 0;
}
