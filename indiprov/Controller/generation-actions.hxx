#ifndef GENERATION_ACTIONS_HXX
#define GENERATION_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/generation.hxx"
#include "model/generation-odb.hxx"

#include "model/entity.hxx"
#include "model/entity-odb.hxx"
#include "model/activity.hxx"
#include "model/activity-odb.hxx"

using namespace std;
using namespace odb::core;

int createGeneration(auto_ptr<database>& db, string entityName, long activityId) {
	transaction t(db->begin());

	shared_ptr<Entity> entity(
		db->query_one<Entity>(query<Entity>::name == entityName));
	if (entity.get() == 0) {
		cerr << "No such entity!";
		t.commit();
		return -1;
	}

	shared_ptr<Activity> activity(
		db->query_one<Activity>(query<Activity>::id == activityId));
	if (activity.get() == 0) {
		cerr << "No such activity!";
		t.commit();
		return -1;
	}

	Generation attribution(entity, activity);
	long id = db->persist(attribution);
	t.commit();
	return id;
}

bool deleteGeneration(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<Generation> generation(
		db->query_one<Generation>(query<Generation>::id == id));
	if (generation.get() != 0) {
		db->erase<Generation>(*generation);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif // GENERATION_ACTIONS_HXX