#ifndef _iPhysicalObject_HG_
#define _iPhysicalObject_HG

#include "cPhysicalProperties.h"

// This is the cGameObject interface that the "physics" updater can "see"
//	into the cGameObject without knowing all about the cGameObject.

// Pure virtual interface
class iPhysicalObject
{
public:

	virtual ~iPhysicalObject() {};			// Make a virtual destructor on any interface (prevents memory leak)
	// Note the virtual and the "= 0". If ALL of these 
	// methods have this, then it's a "pure virtual" or 
	// "interface class"
	virtual void SetPhysProps( cPhysicalProperties &PhysProps ) = 0;
	virtual cPhysicalProperties GetPhysProps( void ) = 0;
	virtual void GetPhysProps( cPhysicalProperties &PhysProps ) = 0;
};



#endif