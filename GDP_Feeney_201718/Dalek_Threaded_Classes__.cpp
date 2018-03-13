#include "Dalek_Threaded.h"

#include <sstream>
#include <process.h>

// Is private, so can't call this
cDalekState::cDalekState()
{
	return;
}

cDalekState::cDalekState(unsigned int DalekID)
{
	this->bIsAlive = true;
	this->velocity = glm::vec3(0.0f);
	this->target = glm::vec3(0.0f);
	this->position = glm::vec3(0.0f);
	this->timeLeftToMove = 0.0f;

	this->minTimeLeftToMove = 0.0f;
	this->maxTimeLeftToMove = 0.0;
	this->minDistanceToMove = glm::vec3(0.0f);
	this->maxDistanceToMove = glm::vec3(0.0f);


	this->threadHandle = 0;
	this->threadID = 0;
	// This is basically the index of the Dalek in the array
	this->DalekID_or_Index = DalekID;

	this->pDalekGO = NULL;

	this->m_pTheDM = NULL;

	return;
}

void cDalekState::setDalekManager(cDalekManager* pTheDM)
{
	this->m_pTheDM = pTheDM;
	return;
}

cDalekManager* cDalekState::getDalekManagerPointer(void)
{
	return this->m_pTheDM;
}



// returns "DalekXXX" where XXX is the DalekID
std::string cDalekState::getFriendlyName(void)
{
	std::stringstream ssName;
	ssName << "Dalek" << this->DalekID_or_Index;
	return ssName.str();
}


cDalekManager::cDalekManager()
{
	InitializeCriticalSection(&( this->m_CS_bufferIsLocked ));

	this->m_bBufferIsLocked = false;

	// Starts at 0
	this->m_nextDalekID = 0;


	return;
}

cDalekManager::~cDalekManager()
{
	DeleteCriticalSection(&( this->m_CS_bufferIsLocked ));

	return;
}

void cDalekManager::m_LockBuffer(void)
{
	EnterCriticalSection(&( this->m_CS_bufferIsLocked ));
	this->m_bBufferIsLocked = true;
	return;
}

void cDalekManager::m_UnlockBuffer(void)
{
	this->m_bBufferIsLocked = false;
	LeaveCriticalSection(&( this->m_CS_bufferIsLocked ));
	return;
}

void cDalekManager::setDalekState(const cDalekState& updatedState)
{
	this->m_vecDalekState[this->m_currentReadBuffer][updatedState.DalekID_or_Index] = updatedState;
	return;
}

void cDalekManager::getDalekLastStateAtIndex(cDalekState& dalekState)
{
	// Read the particular Dalek 
	//	...from the particular buffer
	//	at the particular index...
	dalekState = this->m_vecDalekState[this->m_currentReadBuffer][dalekState.DalekID_or_Index];

	return;
}

// Much slower, linear search for the Dalek
bool cDalekManager::getDalekLastState(cDalekState& dalekState)
{
	for (std::vector< cDalekState >::iterator itDS = this->m_vecDalekState[this->m_currentReadBuffer].begin();
		 itDS != this->m_vecDalekState[this->m_currentReadBuffer].end(); itDS++)
	{
		if (itDS->DalekID_or_Index = dalekState.DalekID_or_Index)
		{	// Found it
			dalekState = *itDS;
			return true;
		}
	}// for ( std::vector< cDalekState >::iterator itDS
	return false;
}


// useful for getting information about location of other Daleks, etc.
void cDalekManager::getDalekLastStates(std::vector<cDalekState> &vecDaleks)
{
	vecDaleks.clear();

	for (std::vector< cDalekState >::iterator itDS = this->m_vecDalekState[this->m_currentReadBuffer].begin();
		 itDS != this->m_vecDalekState[this->m_currentReadBuffer].end(); itDS++)
	{
		vecDaleks.push_back(*itDS);
	}// for ( std::vector< cDalekState >::iterator itDS
	return;
}


// Locks the vectors and copies over the buffer
void cDalekManager::SwitchToNewBuffer(void)
{
	this->m_LockBuffer();
	// There are 4 buffers. 
	// - make the current write buffer the read 
	// - make the 'next' index the new write buffer
	unsigned int nextWriteBuffer = this->m_currentWriteBuffer + 1;
	if (nextWriteBuffer > cDalekManager::NUMBEROFBUFFERS)
	{
		nextWriteBuffer = 0;	// rest this index to zero (the circular part)
	}
	// Point the "read" buffer index to the previous "write" buffer
	// Here's The Trick:
	// - There's always a buffer between the "current" and "next", so 
	//   if something is reading from the "old" read buffer, that's OK:
	//   they will just have data one frame old (but there will be no 
	//   contention with read+write.
	// - Same with the write: if they are still writing to the "old" write
	//   buffer, that's OK, as no one is reading it. 
	// - The side effect is that the data we are reading is 2 frames "old" 
	//   (but that's very common in games, and isn't a big deal here, anyway:
	//    is it really a disaster to have the "2 frames old" position of the Dalek?
	unsigned int nextReadBuffer = this->m_currentWriteBuffer;

	this->m_currentReadBuffer = nextReadBuffer;
	this->m_currentWriteBuffer = nextWriteBuffer;
	this->m_UnlockBuffer();
	return;
}

bool cDalekManager::IsBufferLocked(void)
{
	return this->m_bBufferIsLocked;
}

void cDalekManager::LoadInitialDalekData(std::vector<cDalekState> &vecDaleks)
{
	// The lock is to prevent the changing of the buffers during this "long" write
	this->m_LockBuffer();

	std::copy(vecDaleks.begin(), vecDaleks.end(),
			  this->m_vecDalekState[this->m_currentWriteBuffer].begin());

	this->m_UnlockBuffer();
	return;
}

void cDalekManager::CreateDalekAndAddToState(cDalekState& initialState, bool createSuspended /*=false*/)
{
	cDalekState DS = initialState;

	DS.DalekID_or_Index = this->m_nextDalekID++;

	m_mapDalekID_to_ThreadInfo;

	sThreadInfo DalekThread;
	DalekThread.DalekID_or_Index = DS.DalekID_or_Index;

	// Store this new Dalek into all three vectors
	// NOTE: If this was a 2D array, we wouldn't have to do this 
	// (as the locations would already be there)
	for ( int bufferNumber = 0; bufferNumber != cDalekManager::NUMBEROFBUFFERS; bufferNumber++ )
	{
		this->m_vecDalekState[bufferNumber].push_back( DS );
	}


	//DalekThread.threadHandle = _beginthreadex(
	//	NULL,						// Don't change the security permissions for this thread
	//	0,							// 0 = default stack
	//	DalekBrainThread,				// The function we want to call. 
	//	// Note we are getting the address (to get a pointer), then 
	//	//	casting this POINTER to a pointer-to-void
	//	(void*)(&DS), //pDataToPassA,					// Arguement list (or NULL if we are passing VOID)
	//	(createSuspended ? CREATE_SUSPENDED : 0),		// or CREATE_SUSPENDED if it's paused and has to start
	//	&(DalekThread.threadID) );


	DalekThread.threadHandle = CreateThread(
		NULL,
		0,
		DalekBrainThread_w32,
		(void*)(&DS),
		(createSuspended ? CREATE_SUSPENDED : 0),
		&(DalekThread.threadID) );


	// Store the thread information in the map, in case it's needed
	this->m_mapDalekID_to_ThreadInfo[DalekThread.DalekID_or_Index] = DalekThread;


	return;
}

void cDalekManager::ResumeDalekThread(unsigned int DalekID_or_Index)
{
	// This stores the thread handles, etc, but DalekID (index into the vector)
	std::map< unsigned int /*DalekID_or_Index*/, sThreadInfo >::iterator itDT = this->m_mapDalekID_to_ThreadInfo.find( DalekID_or_Index );

	if ( itDT == this->m_mapDalekID_to_ThreadInfo.end() ) 
	{
		return;
	}

	ResumeThread( itDT->second.threadHandle );

	return;
}	

void cDalekManager::ResumeAllDalekThreads(void)
{
	// This stores the thread handles, etc, but DalekID (index into the vector)
	for ( std::map< unsigned int /*DalekID_or_Index*/, sThreadInfo >::iterator itDT = this->m_mapDalekID_to_ThreadInfo.begin(); 
		  itDT != this->m_mapDalekID_to_ThreadInfo.end(); itDT++ )
	{
		ResumeThread( itDT->second.threadHandle );
	}

	return;
}
