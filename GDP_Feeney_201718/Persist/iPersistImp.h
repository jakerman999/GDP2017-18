#ifndef _iPersistImp_HG_
#define _iPersistImp_HG_

#include "cPersist.h"

class iPersistImp
{
public:
	virtual ~iPersistImp() {};




};

class cPersistImpFactory
{
public:
	iPersistImp* createPersistImp(cPersist::ePersistType backendDataStore);

};

#endif
