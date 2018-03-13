#ifndef _DALEK_THREADED_01_HG_
#define _DALEK_THREADED_01_HG_

#include "iDalekManager.h"
#include <process.h>
#include <Windows.h>		// For CRITICAL_SECTION, CreateThread, ResumeThread, etc.
#include "Utilities.h"


// Dalek threading solution-example #1


#include "DalekThreadedCommon.h"


// This is the actual threading function
//unsigned int __stdcall DalekBrainThread_01(void* pInitialData);	// _beginthreadex() format
DWORD WINAPI DalekBrainThread_01(void* pInitialData);				// CreateThread() format



class cDalekManager01 : public iDalekManager
{
public:
	cDalekManager01();
	virtual ~cDalekManager01();

	virtual bool Init(unsigned int numberOfDaleks);

	// Spawns the next Dalek, using the game object position
	// returns false if there aren't enough 'slots' for the Daleks
	virtual bool CreateDalekThread( cGameObject* pGameObject, cDalek *pDalek );

	// All of these call lock and unlock
	virtual bool getDalekPositionAtIndex(unsigned int index, glm::vec3 &position);

	// Called by each Dalek...
	virtual bool setDalekPositionAtIndex(unsigned int index, glm::vec3 position);
	// Called by Daleks and the main render loop
	virtual bool getDalekPositionsAtIndexRange( unsigned int startIndex, unsigned int endIndex, 
									    std::vector<glm::vec3> &vecDalekPositions );

	virtual bool IsDataLocked(void);

private:
	void m_LockDalekData(void);
	void m_UnlockDalekData(void);

	unsigned int m_NumberOfDaleks;// = 500;

	bool m_bIsLocked;

	CRITICAL_SECTION m_cs_DalekDataLock;

	std::vector<glm::vec3> m_vecDalekPosition;

	// Saves the thread handle, etc.
	std::vector<sDalekThreadInfo> m_vecDalekThreadInfo;

	inline bool m_IsIndexInRange(unsigned int index);

	// When creating a Dalek, this is the next index in the array (vector)
	unsigned int m_nextDalekID;// = 0;
};

#endif
