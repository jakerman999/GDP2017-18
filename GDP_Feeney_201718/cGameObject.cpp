#include "cGameObject.h"


// Start the unique IDs at 1. Why not?
/*static*/ unsigned int cGameObject::m_nextUniqueID = 1;

cGameObject::cGameObject()
{
	this->scale = 1.0f;	// (not zero)
	this->position = glm::vec3(0.0f);
	this->orientation = glm::vec3(0.0f);
	this->orientation2 = glm::vec3(0.0f);

	this->vel = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);	

	// If you aren't sure what the 4th value should be, 
	//	make it a 1.0f ("alpha" or transparency)
	this->diffuseColour = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );

	//Assume everything is simulated 
	this->bIsUpdatedInPhysics = true; //??? 
	this->radius = 0.0f;	// Is this the best value??? Who knows?

	this->typeOfObject = eTypeOfObject::UNKNOWN;	// Technically non-standard
	//this->typeOfObject = UNKNOWN;

	this->bIsWireFrame = false;

	// Set all texture blend values to 0.0f (meaning NO texture)
	for (int index = 0; index != NUMTEXTURES; index++)
	{
		this->textureBlend[index] = 0.0f;
	}

	// Assign unque ID, the increment for next created object
	// (Note: if you write your own copy constructor, be sure to COPY this
	//	value, rather than generate a new one - i.e. call the c'tor again)
	this->m_UniqueID = cGameObject::m_nextUniqueID++;

	return;
}

cGameObject::~cGameObject()
{
	return;
}


