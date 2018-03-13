// DalekThreadedCommon

#include "cGameObject.h"
#include "DalekThreadedCommon.h"

cRandThreaded* g_pThreadedRandom = NULL;

cGameObject* MakeDalekGameObject(glm::vec3 position)
{
	cGameObject* pDalek = new cGameObject();
	pDalek->friendlyName = "Big D";
	cPhysicalProperties physState;
	physState.position = glm::vec3(getRandInRange(-100, 100), 0.0, getRandInRange(-100, 100));
	physState.setOrientationEulerAngles(glm::vec3(0.0, 0.0, 0.0f));
	pDalek->SetPhysState(physState);
	sMeshDrawInfo meshInfo;
	meshInfo.scale = 1.0;
	meshInfo.name = "dalek2005_xyz_uv_res_2.ply";
	meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("GuysOnSharkUnicorn.bmp", 0.0f));
	meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Utah_Teapot_xyz_n_uv_Enterprise.bmp", 1.0f));
	meshInfo.setMeshOrientationEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f), true);
	pDalek->vecMeshes.push_back(meshInfo);

	return pDalek;
}