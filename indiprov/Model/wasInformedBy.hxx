#ifndef WASINFORMEDBY_HXX
#define WASINFORMEDBY_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "activity.hxx"

#pragma db object
class WasInformedBy {

public:
	//informedEntity wasInformedBy informingEntity
	WasInformedBy(shared_ptr<Activity> informedActivity, shared_ptr<Activity> informingActivity) {
		informedActivity_ = informedActivity;
		informingActivity_ = informingActivity;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	WasInformedBy() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Activity> informedActivity_;

#pragma db not_null
	shared_ptr<Activity> informingActivity_;

};

#endif // WASINFORMEDBY_HXX
