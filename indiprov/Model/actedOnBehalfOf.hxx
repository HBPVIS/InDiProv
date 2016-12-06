#ifndef ACTEDONBEHALFOF_HXX
#define ACTEDONBEHALFOF_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "agent.hxx"

#pragma db object
class ActedOnBehalfOf {

public:
	//derivedEntity wasDerivedFrom sourceEntity
	ActedOnBehalfOf(shared_ptr<Agent> actingAgent, shared_ptr<Agent> commandingAgent) {
		actingAgent_ = actingAgent;
		commandingAgent_ = commandingAgent;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	ActedOnBehalfOf() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Agent> actingAgent_;

#pragma db not_null
	shared_ptr<Agent> commandingAgent_;

};

#endif // ACTEDONBEHALFOF_HXX
