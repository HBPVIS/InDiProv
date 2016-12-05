#ifndef GENERATION_HXX
#define GENERATION_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "entity.hxx"
#include "activity.hxx"

#pragma db object
class Generation {

public:
	//Entity wasGeneratedBy Activity
	Generation(shared_ptr<Entity> entity, shared_ptr<Activity> activity) {
		entity_ = entity;
		activity_ = activity;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	Generation() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Entity> entity_;

#pragma db not_null
	shared_ptr<Activity> activity_;

};

#endif // GENERATION_HXX
