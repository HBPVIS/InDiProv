#ifndef WASGENERATEDBY_HXX
#define WASGENERATEDBY_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "entity.hxx"
#include "activity.hxx"

#pragma db object
class WasGeneratedBy {

public:
	//Entity wasGeneratedBy Activity
	WasGeneratedBy(shared_ptr<Entity> entity, shared_ptr<Activity> activity) {
		entity_ = entity;
		activity_ = activity;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	WasGeneratedBy() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Entity> entity_;

#pragma db not_null
	shared_ptr<Activity> activity_;

};

#endif // WASGENERATEDBY_HXX
