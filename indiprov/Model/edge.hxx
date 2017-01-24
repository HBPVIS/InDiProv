#ifndef EDGE_HXX
#define EDGE_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "vertex.hxx"
enum edgeType {
	wasGeneratedBy,
	wasDerivedFrom,
	wasAttributedTo,
	used,
	wasInformedBy,
	wasAssociatedWith,
	actedOnBehalfOf
};

static vertexType getVertexType(edgeType edge, bool firstVertex) {
	switch (edge) {
	case wasGeneratedBy:
		if (firstVertex) {
			return Entity;
		} else {
			return Activity;
		}
	case wasDerivedFrom:
		return Entity;
	case wasAttributedTo:
		if (firstVertex) {
			return Entity;
		} else {
			return Agent;
		}
	case used:
		if (firstVertex) {
			return Activity;
		} else {
			return Entity;
		}
	case wasInformedBy:
		return Activity;
	case wasAssociatedWith:
		if (firstVertex) {
			return Activity;
		} else {
			return Agent;
		}
	case actedOnBehalfOf:
		return Agent;
	}
}

static std::string edgeTypeToString(edgeType edge) {
	switch (edge) {
	case wasGeneratedBy:
		return "wasGeneratedBy";
	case wasDerivedFrom:
		return "wasDerivedFrom";
	case wasAttributedTo:
		return "wasAttributedTo";
	case used:
		return "used";
	case wasInformedBy:
		return "wasInformedBy";
	case wasAssociatedWith:
		return "wasAssociatedWith";
	case actedOnBehalfOf:
		return "actedOnBehalfOf";
	default:
		return "unknown";
	}
}

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
		return edgeTypeToString(type_);
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
