#include "iPersistImp.h"
#include <new>
#include <cassert>	// no stupid C++ 11 static_assert()

// The specific back ends...
#include "cPersistImpSQLite.h"

iPersistImp* cPersistImpFactory::createPersistImp(cPersist::ePersistType backendDataStore)
{
	iPersistImp* pDataStore = nullptr;

	switch (backendDataStore)
	{
	case cPersist::ePersistType::SQLITE:
		pDataStore = new (std::nothrow) cPersistImpSQLite();
		break;
	case cPersist::ePersistType::WINAPPDATA:
		// TODO: This thing
		break;
	case cPersist::ePersistType::TEXT_TRIVIAL:
		// TODO: This thing
		break;

	default:
		// Seriously, what are you doing with your life? 
		assert("Tried to create a backend persist type that doesn't exist.");
		break;

	}

	return pDataStore;
}
