#ifndef ACTEDONBEHALFOF_ACTIONS_HXX
#define ACTEDONBEHALFOF_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/actedOnBehalfOf.hxx"
#include "model/actedOnBehalfOf-odb.hxx"

#include "model/agent.hxx"
#include "model/agent-odb.hxx"

using namespace std;
using namespace odb::core;

int createActedOnBehalfOf(auto_ptr<database>& db, string actingAgentName, string commandingAgentName) {
	transaction t(db->begin());

	shared_ptr<Agent> actingAgent(
		db->query_one<Agent>(query<Agent>::name == actingAgentName));
	if (actingAgent.get() == 0) {
		cerr << "No such derived entity!";
		t.commit();
		return -1;
	}

	shared_ptr<Agent> commandingAgent(
		db->query_one<Agent>(query<Agent>::name == commandingAgentName));
	if (commandingAgent.get() == 0) {
		cerr << "No such source entity!";
		t.commit();
		return -1;
	}

	ActedOnBehalfOf actedOnBehalfOf(actingAgent, commandingAgent);
	long id = db->persist(actedOnBehalfOf);
	t.commit();
	return id;
}

bool deleteActedOnBehalfOf(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<ActedOnBehalfOf> actedOnBehalfOf(
		db->query_one<ActedOnBehalfOf>(query<ActedOnBehalfOf>::id == id));
	if (actedOnBehalfOf.get() != 0) {
		db->erase<ActedOnBehalfOf>(*actedOnBehalfOf);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif // ACTEDONBEHALFOF_ACTIONS_HXX