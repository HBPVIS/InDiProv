#ifndef AGENT_ACTIONS_HXX
#define AGENT_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <../schema/creation_messages.pb.h>

#include "database.hxx" // create_database

#include "model/agent.hxx"
#include "model/agent-odb.hxx"

using namespace std;
using namespace odb::core;

string createAgent(auto_ptr<database>& db, string name) {
	Agent agent(name);
	transaction t(db->begin());
	string id = db->persist(agent);
	t.commit();
	return id;
}

string createPROV(auto_ptr<database>& db, agent_creation msg) {
	return createAgent(db, msg.name());
}

bool deleteAgent(auto_ptr<database>& db, string name) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<Agent> agent(
		db->query_one<Agent>(query<Agent>::name == name));
	if (agent.get() != 0) {
		db->erase<Agent>(*agent);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif AGENT_ACTIONS_HXX