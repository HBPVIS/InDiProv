#ifndef WASASSOCIATEDWITH_ACTIONS_HXX
#define WASASSOCIATEDWITH_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/wasAssociatedWith.hxx"
#include "model/wasAssociatedWith-odb.hxx"

#include "model/activity.hxx"
#include "model/activity-odb.hxx"
#include "model/entity.hxx"
#include "model/entity-odb.hxx"

using namespace std;
using namespace odb::core;

int createWasAssociatedWith(auto_ptr<database>& db, long activityId, string agentName) {
	transaction t(db->begin());

	shared_ptr<Activity> activity(
		db->query_one<Activity>(query<Activity>::id == activityId));
	if (activity.get() == 0) {
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

	WasAssociatedWith wasAssociatedWith(activity, agent);
	long id = db->persist(wasAssociatedWith);
	t.commit();
	return id;
}

bool deleteWasAssociatedWith(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<WasAssociatedWith> wasAssociatedWith(
		db->query_one<WasAssociatedWith>(query<WasAssociatedWith>::id == id));
	if (wasAssociatedWith.get() != 0) {
		db->erase<WasAssociatedWith>(*wasAssociatedWith);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif // WASASSOCIATEDWITH_ACTIONS_HXX