#ifndef USED_ACTIONS_HXX
#define USED_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/used.hxx"
#include "model/used-odb.hxx"

#include "model/activity.hxx"
#include "model/activity-odb.hxx"
#include "model/entity.hxx"
#include "model/entity-odb.hxx"

using namespace std;
using namespace odb::core;

int createUsed(auto_ptr<database>& db, long activityId, string entityName) {
	transaction t(db->begin());

	shared_ptr<Activity> activity(
		db->query_one<Activity>(query<Activity>::id == activityId));
	if (activity.get() == 0) {
		cerr << "No such activity!";
		t.commit();
		return -1;
	}

	shared_ptr<Entity> entity(
		db->query_one<Entity>(query<Entity>::name == entityName));
	if (entity.get() == 0) {
		cerr << "No such entity!";
		t.commit();
		return -1;
	}

	Used used(activity, entity);
	long id = db->persist(used);
	t.commit();
	return id;
}

bool deleteUsed(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<Used> used(
		db->query_one<Used>(query<Used>::id == id));
	if (used.get() != 0) {
		db->erase<Used>(*used);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif // USED_ACTIONS_HXX