#ifndef WASATTRIBUTEDTO_ACTIONS_HXX
#define WASATTRIBUTEDTO_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/wasAttributedTo.hxx"
#include "model/wasAttributedTo-odb.hxx"

#include "model/agent.hxx"
#include "model/agent-odb.hxx"
#include "model/entity.hxx"
#include "model/entity-odb.hxx"

using namespace std;
using namespace odb::core;

int createAttribution(auto_ptr<database>& db, string entityName, string agentName) {
	transaction t(db->begin());

	shared_ptr<Entity> entity(
		db->query_one<Entity>(query<Entity>::name == entityName));
	if (entity.get() == 0) {
		cerr << "No such entity!";
		t.commit();
		return -1;
	}

	shared_ptr<Agent> agent(
		db->query_one<Agent>(query<Agent>::name == agentName));
	if (agent.get() == 0) {
		cerr << "No such agent!";
		t.commit();
		return -1;
	}

	WasAttributedTo attribution(entity, agent);
	long id = db->persist(attribution);
	t.commit();
	return id;
}

bool deleteAttribution(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<WasAttributedTo> attribution(
		db->query_one<WasAttributedTo>(query<WasAttributedTo>::id == id));
	if (attribution.get() != 0) {
		db->erase<WasAttributedTo>(*attribution);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif // WASATTRIBUTEDTO_ACTIONS_HXX