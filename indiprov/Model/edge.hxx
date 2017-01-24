#ifndef EDGE_HXX
#define EDGE_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "vertex.hxx"

enum edgeType { actedOnBehalfOf, wasAttributedTo };

#pragma db object
class Edge {

public:
	Edge(edgeType type, shared_ptr<Vertex> first, shared_ptr<Vertex> second) {
		type_ = type;
		first_ = first;
		second_ = second;
	}

	const edgeType getType() {
		return type_;
	}

	const std::string getTypeString() {
		switch (type_) {
		case actedOnBehalfOf:
			return "actedOnBehalfOf";
		case wasAttributedTo:
			return "wasAttributedTo";
		default:
			return "unknown";
		}
	}

	const long getId() {
		return id_;
	}
	
	shared_ptr<Vertex> getFirst() {
		return first_;
	}

	shared_ptr<Vertex> getSecond() {
		return second_;
	}

private:
	friend class odb::access;

	Edge() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Vertex> first_;

#pragma db not_null
	shared_ptr<Vertex> second_;

	edgeType type_;
	
};

#endif // EDGE_HXX
