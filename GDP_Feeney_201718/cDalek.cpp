#include "cDalek.h"
#include "Utilities.h"	// cRandThreaded

#include <iostream>

// Can't call (is private)
cDalek::cDalek()
{
	return;
}

// These are all static
const float cDalek::DEFAULT_MIN_TIME_TO_MOVE = 1.0;
const float cDalek::DEFAULT_MAX_TIME_TO_MOVE = 5.0;
const float cDalek::DEFAULT_MIN_DISTANCE_TO_MOVE = 1.0;
const float cDalek::DEFAULT_MAX_DISTANCE_TO_MOVE = 25.0;


cDalek::cDalek(unsigned int DalekID_or_Index)
{
	this->position = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);
	this->target = glm::vec3(0.0f);

	this->bIsAlive = true;

	this->minTimeToMove = cDalek::DEFAULT_MIN_TIME_TO_MOVE;
	float maxTimeToMove = cDalek::DEFAULT_MAX_TIME_TO_MOVE;
	float minDistanceToMove = cDalek::DEFAULT_MIN_DISTANCE_TO_MOVE;
	float maxDistanceToMove = cDalek::DEFAULT_MAX_DISTANCE_TO_MOVE;


	this->m_DalekID_or_Index = DalekID_or_Index;

	this->m_pTimer->ResetLongDuration();

	this->m_pDalekManager = NULL;
	this->m_pRand = NULL;

	// Thread information (in case we need it)
	HANDLE threadHandle = 0;
	DWORD threadAddress = 0;


	return;
}

unsigned int cDalek::getDalekID(void)
{
	return this->m_DalekID_or_Index;
}


void cDalek::Update(void)
{

	this->m_pTimer->UpdateLongDuration();

	float deltaTime = this->m_pTimer->get_fLondDurationTotalSeconds();

	if ( this->timeToMove <= 0.0f )
	{	
		// Pick a new direction 
		// Check for collisions, whatever
		
		if ( this->m_pRand )
		{
			this->timeToMove = (float)getRandInRange( 1.0, 5.0, this->m_pRand->getNextRandDouble() );
		}
		
		this->m_pTimer->ResetAndStart();
	}
	else
	{
		std::cout << "Dalek " << this->m_DalekID_or_Index << " picking new path" << std::endl;

		// Move
		this->position += this->velocity * deltaTime;

		this->timeToMove -= deltaTime;
		// Update this in the Dalek Manager

		// This updates the Dalek's location in the central store of positions.
		// (the one that is thread safe)
		if ( this->m_pDalekManager )
		{
			this->m_pDalekManager->setDalekPositionAtIndex( this->m_DalekID_or_Index, this->position );
		}
	}

	// Check for collisions


	return;
}

void cDalek::setDalkeManager(iDalekManager* pDM)
{
	this->m_pDalekManager = pDM;
	return;
}

void cDalek::setRandThreaded(cRandThreaded* pRT)
{
	this->m_pRand = pRT;
	return;
}
