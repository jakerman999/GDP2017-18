#include "Dalek_Threaded_01.h"

#include "cGameObject.h"
#include "cDalek.h"

// This is the actual threading function
DWORD WINAPI DalekBrainThread_01(void* pInitialData)	// CreateThread() format
{
	cDalek* pDalek = (cDalek*)(pInitialData);

	while ( pDalek->bIsAlive )
	{
		// That's it: calls the Update on the object that we passed in...
		pDalek->Update();

	}//while ( pDalek->bIsAlive )

	return 0;
}


cDalekManager01::cDalekManager01()
{
	InitializeCriticalSection( &(this->m_cs_DalekDataLock) );

	if ( ::g_pThreadedRandom )
	{
		::g_pThreadedRandom = new cRandThreaded();
	}

	this->m_nextDalekID = 0;

	return;
}

bool cDalekManager01::CreateDalekThread(cGameObject* pGameObject, cDalek *pDalek)
{
	if ( this->m_nextDalekID >= this->m_vecDalekPosition.size() )
	{	// Too many Daleks
		return false;
	}

	cDalek* pNewDalek = new cDalek(this->m_nextDalekID);
	this->m_nextDalekID++;
	pNewDalek->setDalkeManager( this );
	pNewDalek->setRandThreaded( g_pThreadedRandom );

	sDalekThreadInfo threadInfo;

	threadInfo.handle = CreateThread( 
	                NULL, // Attributes
		            0,		// 0 = default stack size,
	                DalekBrainThread_01, 
	                (void*)pNewDalek,
	                0,			// 0 or CREATE_SUSPENDED, 
	                &(threadInfo.address) );
//	DalekBrainThread_01

	return true;
}



bool cDalekManager01::Init(unsigned int numberOfDaleks)
{
	this->m_LockDalekData();

	this->m_NumberOfDaleks = numberOfDaleks;

	// Load up positions for 500 Daleks... 
	// The values aren't important as they will be overwritten by the Daleks themselves, later
	for ( int count = 0; count != this->m_NumberOfDaleks; count++ )
	{
		// Make an "empty" Dalek at this index...
		this->m_vecDalekPosition.push_back(glm::vec3(0.0f));
		// ...and make an "empty" thread info at this index, too
		this->m_vecDalekThreadInfo.push_back( sDalekThreadInfo() );
	}
	this->m_UnlockDalekData();

	return true;
}


cDalekManager01::~cDalekManager01()
{
	DeleteCriticalSection( &(this->m_cs_DalekDataLock) );
	return;
}

//inline 
bool cDalekManager01::m_IsIndexInRange(unsigned int index)
{
	if ( index < (unsigned int)this->m_vecDalekPosition.size() )
	{
		return true;
	}
	return false;
}


// From iDalekManager
bool cDalekManager01::getDalekPositionAtIndex(unsigned int index, glm::vec3 &position)
{
	if ( ! this->m_IsIndexInRange(index) )
	{
		// Index is out of range
		return false;
	}

	this->m_LockDalekData();
	position = this->m_vecDalekPosition[index];
	this->m_UnlockDalekData();

	return true;
}

// From iDalekManager
// Is only called by the Daleks... 
bool cDalekManager01::setDalekPositionAtIndex(unsigned int index, glm::vec3 position)
{
	if (!this->m_IsIndexInRange(index))
	{
		// Index is out of range
		return false;
	}

	this->m_LockDalekData();
	this->m_vecDalekPosition[index] = position;
	this->m_UnlockDalekData();

	return true;
}

// From iDalekManger
bool cDalekManager01::getDalekPositionsAtIndexRange(unsigned int startIndex, unsigned int endIndex,
											   std::vector<glm::vec3> &vecDalekPositions)
{
	if ( (!this->m_IsIndexInRange(startIndex)) || 
		 (!this->m_IsIndexInRange(endIndex)) )
	{
		// Index is out of range
		return false;
	}

	vecDalekPositions.clear();

	this->m_LockDalekData();
	for ( unsigned int index = startIndex; index > endIndex; index++ )
	{
		vecDalekPositions[index] = this->m_vecDalekPosition[index];
	}
	// Or...
	//std::copy( this->m_vecDalekPosition.begin(), this->m_vecDalekPosition.end(), vecDalekPositions );
	this->m_UnlockDalekData();

	return true;
}

// From iDalekManger
bool cDalekManager01::cDalekManager01::IsDataLocked(void)
{
	return this->m_bIsLocked;
}

// ******************************************

void cDalekManager01::m_LockDalekData(void)
{
	EnterCriticalSection( &(this->m_cs_DalekDataLock) );
	this->m_bIsLocked = true;
	return;
}

void cDalekManager01::m_UnlockDalekData(void)
{
	this->m_bIsLocked = false;
	LeaveCriticalSection( &(this->m_cs_DalekDataLock) );
	return;
}


// The brain is now passed the entire cDalekState object
unsigned int __stdcall DalekBrainThread(void* pInitialData)
{
	//// The DalekState is passed via pointer
	//cDalekState* pDS = ( cDalekState* )( whatWePassed );	// Pointer-to-void to pointer-to-cDalekState

	//														// Deference pointer to copy state locally
	//cDalekState threadLocalDalekState = *pDS;

	//while (threadLocalDalekState.bIsAlive)
	//{
	//	if (threadLocalDalekState.timeLeftToMove >= 0.0f)
	//	{
	//		// Move the Dalek
	//		double deltaTime = ::g_GetDeltaTime();	// Thread safe function

	//		threadLocalDalekState.position += ( threadLocalDalekState.velocity * ( float )deltaTime );

	//		threadLocalDalekState.timeLeftToMove -= ( float )deltaTime;

	//		// Do we have a manager to update? 
	//		if (threadLocalDalekState.getDalekManagerPointer())
	//		{
	//			threadLocalDalekState.getDalekManagerPointer()->setDalekState(threadLocalDalekState);
	//		}

	//		// Are we colliding with any other Dalek? 
	//		// read from the vecDalekLastFrame vector to see 

	//		// Go away for x ms
	//		Sleep(1);
	//	}
	//	else
	//	{
	//		// pick another destination
	//		//g_DalekTimeLeftToMove = getRandInRange<double>(1.0, 10.0);

	//		double dblRand = g_ThreadedRand.getNextRandDouble();

	//		threadLocalDalekState.timeLeftToMove
	//			= getRandInRange<float>(threadLocalDalekState.minTimeLeftToMove,
	//									threadLocalDalekState.maxTimeLeftToMove,
	//									( float )dblRand);


	//		const float MAXDALEKMOVEMENT = 50.0f;

	//		// Pick a distance within the range:
	//		threadLocalDalekState.target.x =
	//			getRandInRange<float>(threadLocalDalekState.minDistanceToMove.x,
	//								  threadLocalDalekState.maxDistanceToMove.x,
	//								  ( float )g_ThreadedRand.getNextRandDouble());
	//		// +ve or -ve?
	//		if (g_ThreadedRand.getNextRandDouble() > 0.5)
	//		{
	//			threadLocalDalekState.target.x = -threadLocalDalekState.target.x;
	//		}

	//		threadLocalDalekState.target.y = 0.0;

	//		threadLocalDalekState.target.z =
	//			getRandInRange<float>(threadLocalDalekState.minDistanceToMove.z,
	//								  threadLocalDalekState.maxDistanceToMove.z,
	//								  ( float )g_ThreadedRand.getNextRandDouble());
	//		// +ve or -ve?
	//		if (g_ThreadedRand.getNextRandDouble() > 0.5)
	//		{
	//			threadLocalDalekState.target.z = -threadLocalDalekState.target.z;
	//		}

	//		threadLocalDalekState.velocity.x = ( float )( threadLocalDalekState.target.x / threadLocalDalekState.timeLeftToMove );
	//		//			g_DalekVelocity.y = g_DalekTarget.y / ( (float)g_DalekTimeLeftToMove * 10.0);
	//		threadLocalDalekState.velocity.y = 0.0;
	//		threadLocalDalekState.velocity.z = ( float )( threadLocalDalekState.target.z / threadLocalDalekState.timeLeftToMove );

	//		// Not really needed since we have velocity and time
	//		//glm::vec3 theDalekPosition = ::g_GetDalekPosition();
	//		//g_DalekTarget = theDalekPosition += g_DalekTarget;

	//	}
	//}//while (g_bDalekIsAlive)

	return 0;
}
