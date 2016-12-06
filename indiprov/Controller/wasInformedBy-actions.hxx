#ifndef WASINFORMEDBY_ACTIONS_HXX
#define WASINFORMEDBY_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/wasInformedBy.hxx"
#include "model/wasInformedBy-odb.hxx"

#include "model/activity.hxx"
#include "model/activity-odb.hxx"

using namespace std;
using namespace odb::core;

int createWasInformedBy(auto_ptr<database>& db, long informedActivityId, long informingActivityId) {
	transaction t(db->begin());

	shared_ptr<Activity> informedActivity(
		db->query_one<Activity>(query<Activity>::id == informedActivityId));
	if (informedActivity.get() == 0) {
		cerr << "No such informed activity!";
		t.commit();
		return -1;
	}

	shared_ptr<Activity> informingActivity(
		db->query_one<Activity>(query<Activity>::id == informingActivityId));
	if (informingActivity.get() == 0) {
		cerr << "No such informing activity!";
		t.commit();
		return -1;
	}

	WasInformedBy wasInformedBy(informedActivity, informingActivity);
	long id = db->persist(wasInformedBy);
	t.commit();
	return id;
}

bool deleteWasInformedBy(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<WasInformedBy> wasInformedBy(
		db->query_one<WasInformedBy>(query<WasInformedBy>::id == id));
	if (wasInformedBy.get() != 0) {
		db->erase<WasInformedBy>(*wasInformedBy);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif // WASINFORMEDBY_ACTIONS_HXX