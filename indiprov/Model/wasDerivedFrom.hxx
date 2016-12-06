#ifndef WASDERIVEDFROM_HXX
#define WASDERIVEDFROM_HXX

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include <odb/core.hxx>

#include "entity.hxx"

#pragma db object
class WasDerivedFrom {

public:
	//derivedEntity wasDerivedFrom sourceEntity
	WasDerivedFrom(shared_ptr<Entity> derivedEntity, shared_ptr<Entity> sourceEntity) {
		derivedEntity_ = derivedEntity;
		sourceEntity_ = sourceEntity;
	}

	const long getId() {
		return id_;
	}

private:
	friend class odb::access;

	WasDerivedFrom() {}

#pragma db id auto
	unsigned long id_;

#pragma db not_null
	shared_ptr<Entity> derivedEntity_;

#pragma db not_null
	shared_ptr<Entity> sourceEntity_;

};

#endif // WASDERIVEDFROM_HXX
