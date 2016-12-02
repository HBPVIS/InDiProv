#ifndef WASATTRIBUTEDTO_HXX
#define WASATTRIBUTEDTO_HXX

#include <string>
#include <cstddef> // std::size_t

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "agent.hxx"
#include "entity.hxx"

#pragma db object
class Attribution {

public:
	Attribution(shared_ptr<Entity> entity, shared_ptr<Agent> agent)  {
		entity_ = entity;
		agent_ = agent;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	Attribution() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Entity> entity_;

#pragma db not_null
	shared_ptr<Agent> agent_;

};

#endif // WASATTRIBUTEDTO_HXX
