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

private:
	friend class odb::access;

	Agent() {}

#pragma db id
	std::string name_;

};

#endif // AGENT_HXX
