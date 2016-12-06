#ifndef WASASSOCIATEDWITH_HXX
#define WASASSOCIATEDWITH_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "activity.hxx"
#include "agent.hxx"

#pragma db object
class WasAssociatedWith {

public:
	//Activity wasAssociatedWith Agent
	WasAssociatedWith(shared_ptr<Activity> activity, shared_ptr<Agent> agent)  {
		activity_ = activity;
		agent_ = agent;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	WasAssociatedWith() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Activity> activity_;

#pragma db not_null
	shared_ptr<Agent> agent_;

};

#endif // WASASSOCIATEDWITH_HXX
