#ifndef ACTIVITY_HXX
#define ACTIVITY_HXX

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>

#pragma db object
class activity
{
public:
	activity(const std::string& name,
		unsigned long start,
		unsigned long end) 
		: name_(name), start_(start), end_(end) {	}

	const std::string& name() const {
		return name_;
	}

	unsigned long start() const {
		return start_;
	}

	unsigned long end() const {
		return end_;
	}

private:
	friend class odb::access;

	activity() {}

#pragma db id auto
	unsigned long id_;

	std::string name_;
	unsigned long start_;
	unsigned long end_;

};

#endif // ACTIVITY_HXX
