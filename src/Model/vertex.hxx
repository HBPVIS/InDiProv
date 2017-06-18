#ifndef VERTEX_HXX
#define VERTEX_HXX

#include <string>
#include <cstddef> // std::size_t
#include <iostream> //for namecheck

#include <odb/core.hxx>

enum vertexType {
	Agent = 0,
	Activity = 1,
	Entity = 2
};

const int NUM_VERTEX_TYPES = 3;

static std::string vertexTypeToString(vertexType vertex) {
	switch (vertex) {
	case Agent:
		return "Agent";
	case Activity:
		return "Activity";
	case Entity:
		return "Entity";
	default:
		return "unknown";
	}
}

static vertexType stringToVertexType(std::string vertex) {
	for(vertexType type = vertexType(0); type < NUM_VERTEX_TYPES; type = vertexType(type+1)) {
		if(vertex == vertexTypeToString(type)) {
			return type;
		}
	}
	throw ("vertexType " + vertex + " not implemented.");
}

#pragma db object
class Vertex {

public:
	Vertex() {}

	Vertex(
		const std::string& client,
		const std::string& session,
		vertexType type,
		const std::string& name,
		unsigned long start,
		unsigned long end)
		: client_(client), session_(session), type_(type), name_(name), start_(start), end_(end) {	}

	const std::string& GetClient() {
		return client_;
	}

	const std::string& GetSession() {
		return session_;
	}

	const vertexType GetType() {
		return type_;
	}

	const std::string GetTypeString() {
		return vertexTypeToString(type_);
	}

	const std::string& GetName() {
		return name_;
	}

	unsigned long GetStart() {
		return start_;
	}

	unsigned long GetEnd() {
		return end_;
	}

	unsigned GetId() {
		return id_;
	}

private:
	friend class odb::access;

	#pragma db id auto
	unsigned long id_;

	#pragma db type("VARCHAR(128)")
	std::string client_;
	#pragma db type("VARCHAR(128)")
	std::string session_;
	vertexType type_;
	#pragma db type("VARCHAR(128)")
	std::string name_;
	unsigned long start_;
	unsigned long end_;

	#pragma db index("client_name") type("UNIQUE") members(client_, name_)
};

#endif // VERTEX_HXX
