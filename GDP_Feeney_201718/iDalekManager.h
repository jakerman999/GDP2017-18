#ifndef _iDALEK_MANAGER_HG_
#define _iDALEK_MANAGER_HG_

#include <glm/glm.hpp>
#include <vector> 

class iDalekManager
{
public:
	virtual ~iDalekManager() {};

	virtual bool Init( unsigned int numberOfDaleks ) = 0;

	virtual bool getDalekPositionAtIndex(unsigned int index, glm::vec3 &position) = 0;
	virtual bool setDalekPositionAtIndex(unsigned int index, glm::vec3 position) = 0;
	virtual bool getDalekPositionsAtIndexRange(unsigned int startIndex, unsigned int endIndex,
									           std::vector<glm::vec3> &vecDalekPositions) = 0;
	virtual bool IsDataLocked(void) = 0;
};

#endif

