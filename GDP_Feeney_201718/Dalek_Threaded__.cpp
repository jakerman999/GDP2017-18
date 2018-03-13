// Dalek_threaded

#include "Utilities.h"
#include <Windows.h>		// Critical section stuff


cRandThreaded g_ThreadedRand;

//bool g_bDalekIsAlive = true;
//
//glm::vec3 g_DalekVelocity;//  = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 g_DalekTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//double g_DalekTimeLeftToMove = -1.0;
//
// Being read by outside things
glm::vec3 fg_DalekPosition = glm::vec3(0.0f, 0.0f, 0.0f);
double fg_DeltaTime = 0.0;



CRITICAL_SECTION CS_DeltaTime;
//CRITICAL_SECTION CS_DalekPosition;

void InitDalekThreading(void)
{
	InitializeCriticalSection( &CS_DeltaTime );
	//InitializeCriticalSection( &CS_DalekPosition );

	return;
}

//glm::vec3 g_GetDalekPosition(void)
//{
//	EnterCriticalSection( &CS_DalekPosition );
//	glm::vec3 lastGoodDalekPosition = fg_DalekPosition;
//	LeaveCriticalSection( &CS_DalekPosition ); 
//
//	return lastGoodDalekPosition;
//}
//
//void g_SetDalekPosition(glm::vec3 newDalekPosition)
//{
//	EnterCriticalSection( &CS_DalekPosition );
//	fg_DalekPosition = newDalekPosition;
//	LeaveCriticalSection( &CS_DalekPosition );
//	return;
//}


void g_SetDeltaTime(double newTime)
{
	EnterCriticalSection( &CS_DeltaTime );
	fg_DeltaTime = newTime;
	LeaveCriticalSection( &CS_DeltaTime );
	return;
}

double g_GetDeltaTime(void)
{
	EnterCriticalSection( &CS_DeltaTime );
	double theLastGoodDeltaTime = fg_DeltaTime;
	LeaveCriticalSection( &CS_DeltaTime );

	return theLastGoodDeltaTime;
}


// The brain is now passed the entire cDalekState object
unsigned int __stdcall DalekBrainThread(void* whatWePassed)
{
	// The DalekState is passed via pointer
	cDalekState* pDS = (cDalekState*)( whatWePassed );	// Pointer-to-void to pointer-to-cDalekState

	// Deference pointer to copy state locally
	cDalekState threadLocalDalekState = *pDS;

	while (threadLocalDalekState.bIsAlive)
	{
		if (threadLocalDalekState.timeLeftToMove >= 0.0f )
		{
			// Move the Dalek
			double deltaTime = ::g_GetDeltaTime();	// Thread safe function

			threadLocalDalekState.position += ( threadLocalDalekState.velocity * (float)deltaTime );

			threadLocalDalekState.timeLeftToMove -= (float)deltaTime;

			// Do we have a manager to update? 
			if ( threadLocalDalekState.getDalekManagerPointer() )
			{
				threadLocalDalekState.getDalekManagerPointer()->setDalekState(threadLocalDalekState);
			}

			// Are we colliding with any other Dalek? 
			// read from the vecDalekLastFrame vector to see 

			// Go away for x ms
			Sleep(1);
		}
		else
		{
			// pick another destination
			//g_DalekTimeLeftToMove = getRandInRange<double>(1.0, 10.0);

			double dblRand = g_ThreadedRand.getNextRandDouble();

			threadLocalDalekState.timeLeftToMove 
				= getRandInRange<float>( threadLocalDalekState.minTimeLeftToMove,
				                          threadLocalDalekState.maxTimeLeftToMove,
										  (float)dblRand );

			
			const float MAXDALEKMOVEMENT = 50.0f;
			
			// Pick a distance within the range:
			threadLocalDalekState.target.x = 
				getRandInRange<float>( threadLocalDalekState.minDistanceToMove.x, 
									    threadLocalDalekState.maxDistanceToMove.x, 
									    (float)g_ThreadedRand.getNextRandDouble() );
			// +ve or -ve?
			if ( g_ThreadedRand.getNextRandDouble() > 0.5 )
			{
				threadLocalDalekState.target.x = -threadLocalDalekState.target.x;
			}

			threadLocalDalekState.target.y = 0.0;

			threadLocalDalekState.target.z =
				getRandInRange<float>(threadLocalDalekState.minDistanceToMove.z,
									   threadLocalDalekState.maxDistanceToMove.z,
									   (float)g_ThreadedRand.getNextRandDouble());
			// +ve or -ve?
			if (g_ThreadedRand.getNextRandDouble() > 0.5)
			{
				threadLocalDalekState.target.z = -threadLocalDalekState.target.z;
			}
			
			threadLocalDalekState.velocity.x = (float)(threadLocalDalekState.target.x / threadLocalDalekState.timeLeftToMove);
//			g_DalekVelocity.y = g_DalekTarget.y / ( (float)g_DalekTimeLeftToMove * 10.0);
			threadLocalDalekState.velocity.y = 0.0;
			threadLocalDalekState.velocity.z = (float)(threadLocalDalekState.target.z / threadLocalDalekState.timeLeftToMove);

			// Not really needed since we have velocity and time
			//glm::vec3 theDalekPosition = ::g_GetDalekPosition();
			//g_DalekTarget = theDalekPosition += g_DalekTarget;

		}
	}//while (g_bDalekIsAlive)

	return 0;
}


void ShutDownDalekThreading(void)
{
	DeleteCriticalSection(&CS_DeltaTime);
	//DeleteCriticalSection(&CS_DalekPosition);

	return;
}





