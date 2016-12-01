#ifndef AGENT_ACTIONS_HXX
#define AGENT_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "model/agent.hxx"
#include "model/agent-odb.hxx"

using namespace std;
using namespace odb::core;

int createAgent(auto_ptr<database>& db, string name) {
	Agent agent(name);
	transaction t(db->begin());
	int id = db->persist(agent);
	t.commit();
	return id;
}

bool deleteAgent(auto_ptr<database>& db, string name) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<Agent> agent(
		db->query_one<Agent>(query<Agent>::name == name));
	if (agent.get() != 0) {
		db->erase<Agent>(agent->getId());
		deleted = true;
	}
	t.commit();
	return deleted;
}

long getAgentId(auto_ptr<database>& db, string name) {
	long id = -1;
	transaction t(db->begin());
	auto_ptr<Agent> agent(
		db->query_one<Agent>(query<Agent>::name == name));
	if (agent.get() != 0) {
		id = agent->getId();
	}
	t.commit();
	return id;
}

#endif AGENT_ACTIONS_HXX