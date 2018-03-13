#ifndef _DALEK_THREADED_HG_
#define _DALEK_THREADED_HG_

#include <glm\glm.hpp>
#include <vector>
#include <map>

#include "Utilities.h"



//extern bool g_bDalekIsAlive;
//extern glm::vec3 g_DalekVelocity;
//extern glm::vec3 g_DalekTarget;
//extern double g_DalekTimeLeftToMove;
//glm::vec3 g_DalekPosition;

extern cRandThreaded g_ThreadedRand;

// Forward declare so that we can easily copy the DalakState 
//	into the game objects to be rendered (instead of trying to 
//  "find them" every frame)
// NOTE: We can get away with this in the Dalek state because
//	the pointer to the game object will always be the same. 
//  i.e. even when we copy the read and write buffers, they 
//	will simply copy the same pointer value - there's no way
//	that we will get the "wrong" pointer value, is my point.
//  (of course, if the pointers might change, then this is
//   something that we'd have to be careful about)
class cGameObject;

// Forward declare so that the cDalekState can 
//  contact the manager to update its state
class cDalekManager;

class cDalekState
{
public:
	cDalekState(unsigned int DalekID);
	bool bIsAlive;
	glm::vec3 velocity;
	glm::vec3 target;
	glm::vec3 position;
	float timeLeftToMove;
	// Used for picking not locations to move to
	float minTimeLeftToMove;
	float maxTimeLeftToMove;
	glm::vec3 minDistanceToMove;
	glm::vec3 maxDistanceToMove;
	//
	unsigned int threadHandle;
	unsigned int threadID;
	// This is basically the index of the Dalek in the array
	unsigned int DalekID_or_Index;
	// returns "DalekXXX" where XXX is the DalekID
	std::string getFriendlyName(void);
	cGameObject* pDalekGO;
	// These don't really protect the manager pointer, but
	//	will partially protect against mistaken assignment
	void setDalekManager(cDalekManager* pTheDM);
	cDalekManager* getDalekManagerPointer(void);
private:
	// Don't allow a "blank" Dalek
	cDalekState();
	cDalekManager* m_pTheDM;
};



class cDalekManager
{
public:
	cDalekManager();
	~cDalekManager();

	// Locks the vectors and copies over the buffer
	void SwitchToNewBuffer(void);

	// Will load DakekID (index into vector)
	// Can pass starting location, etc., too
	void CreateDalekAndAddToState(cDalekState& initialState, bool createSuspended = false);
	void ResumeDalekThread(unsigned int DalekID_or_Index);
	void ResumeAllDalekThreads(void);
	// Alternative way to do this
	void LoadInitialDalekData( std::vector<cDalekState> &vecDaleks );

	// The DalekID passed is used to locate the Dalek
	// NOTE: Does NOT check for bad index locations!
	void getDalekLastStateAtIndex( cDalekState& dalekState );
	// Much slower, linear search for the Dalek
	bool getDalekLastState( cDalekState& dalekState );
	// useful for getting information about location of other Daleks, etc.
	void getDalekLastStates( std::vector<cDalekState> &vecDaleks );
	// Note: the Daleks are maintaining their own state, 
	//	but are writing to this buffer to update it
	// (i.e. they aren't reading and writing)
	void setDalekState( const cDalekState& updatedState );

	bool IsBufferLocked(void);

private:
	unsigned int m_currentReadBuffer;
	unsigned int m_currentWriteBuffer;
	static const int NUMBEROFBUFFERS = 4;
	std::vector< cDalekState > m_vecDalekState[NUMBEROFBUFFERS];

	struct sThreadInfo
	{
		sThreadInfo() : DalekID_or_Index(0), threadHandle(0), threadID(0) {}; /*, threadIdentifier(0)*/ 
		unsigned int DalekID_or_Index;
		//unsigned long threadHandle;
		//unsigned int threadID;
		HANDLE threadHandle;
		DWORD threadID;
	};
	// This stores the thread handles, etc, but DalekID (index into the vector)
	std::map< unsigned int /*DalekID_or_Index*/, sThreadInfo > m_mapDalekID_to_ThreadInfo;

	// Starts at 0
	unsigned int m_nextDalekID;

	CRITICAL_SECTION m_CS_bufferIsLocked;
	void m_LockBuffer(void);
	void m_UnlockBuffer(void);
	bool m_bBufferIsLocked;
};


unsigned int __stdcall DalekBrainThread(void* whatWePassed);

DWORD WINAPI DalekBrainThread_w32( void* lpParameter );


void InitDalekThreading(void);

// Used by thread for last update
//extern double g_DeltaTime;
void g_SetDeltaTime(double newTime);
double g_GetDeltaTime(void);

//extern glm::vec3 g_DalekPosition;
//glm::vec3 g_GetDalekPosition(void);
//void g_SetDalekPosition(glm::vec3 newDalekPosition);


#endif