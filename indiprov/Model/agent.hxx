#ifndef AGENT_HXX
#define AGENT_HXX

#include <string>
#include <cstddef> // std::size_t
#include <iostream> //for namecheck

#include <odb/core.hxx>

#pragma db object
class Agent {

public:
	Agent(const std::string& name) : name_(name) {
		if (name.length() > 128) {
			std::cout << "Name too long";
		}
	}

	const std::string& name() const	{
		return name_;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	Agent() {}

#pragma db id auto
	unsigned long id_;

#pragma db unique
#pragma db type("VARCHAR(128)")
	std::string name_;

};

#endif // AGENT_HXX
