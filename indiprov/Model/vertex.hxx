#ifndef VERTEX_HXX
#define VERTEX_HXX

#include <string>
#include <cstddef> // std::size_t
#include <iostream> //for namecheck

#include <odb/core.hxx>

enum vertexType { Agent, Activity, Entity };

#pragma db object
class Vertex {

public:
	Vertex(vertexType type,
		const std::string& name,
		unsigned long start,
		unsigned long end) 
		: type_(type), name_(name), start_(start), end_(end) {	}
	
	const vertexType type() const {
		return type_;
	}

	const std::string& name() const {
		return name_;
	}

	unsigned long start() const {
		return start_;
	}

	unsigned long end() const {
		return end_;
	}

	unsigned getId() const {
		return id_;
	}

private:
	friend class odb::access;

	Vertex() {}

#pragma db id auto
	unsigned long id_;

	vertexType type_;
	std::string name_;
	unsigned long start_;
	unsigned long end_;

};

#endif // VERTEX_HXX
