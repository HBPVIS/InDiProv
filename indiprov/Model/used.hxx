#ifndef USED_HXX
#define USED_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "activity.hxx"
#include "entity.hxx"

#pragma db object
class Used {

public:
	//Activity used Entity
	Used(shared_ptr<Activity> activity, shared_ptr<Entity> entity) {
		activity_ = activity;
		entity_ = entity;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	Used() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Activity> activity_;

#pragma db not_null
	shared_ptr<Entity> entity_;

};

#endif // USED_HXX
