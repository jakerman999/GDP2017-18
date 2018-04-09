#ifndef _sNUB_skinnedMeshBones_HG_
#define _sNUB_skinnedMeshBones_HG_

// This matches the NUB used in the simpleVertBlendedSkinnedMesh.glsl shader

#include <glm/glm.hpp>		// mat4

//layout(std140) uniform NUB_skinnedMeshBones
//{
//	mat4 bones_pose_0[MAXNUMBEROFBONES];
//	mat4 bones_pose_1[MAXNUMBEROFBONES];
//	//mat4 bones_pose_2[MAXNUMBEROFBONES];	
//	//mat4 bones_pose_3[MAXNUMBEROFBONES];
//
//	// For std140: 
//	// "If the member is a scalar consuming N basic 
//	// "machine units, the base alignment is N."
//	float blendWeight_pose_0[MAXNUMBEROFBONES];
//	float blendWeight_pose_1[MAXNUMBEROFBONES];
//	//float blendWeight_pose_2[MAXNUMBEROFBONES];
//	//float blendWeight_pose_3[MAXNUMBEROFBONES];
//} blendedBones;

#define MAXNUMBEROFBONES 100

struct sNUB_skinnedMeshBones
{
	sNUB_skinnedMeshBones()
	{
		// Clear everything (including the mat4s)
		memset( this, 0, sizeof(sNUB_skinnedMeshBones) );
		// Set bones to identity
		for ( unsigned int index = 0; index != MAXNUMBEROFBONES; index++ )
		{
			this->bones_pose_0[index] = glm::mat4(1.0f);
			this->bones_pose_0[index] = glm::mat4(1.0f);
		}
	}
	glm::mat4 bones_pose_0[MAXNUMBEROFBONES];
	glm::mat4 bones_pose_1[MAXNUMBEROFBONES];
	//mat4 bones_pose_2[MAXNUMBEROFBONES];	
	//mat4 bones_pose_3[MAXNUMBEROFBONES];

	// For std140: 
	// "If the member is a scalar consuming N basic 
	// "machine units, the base alignment is N."
	float blendWeight_pose_0[MAXNUMBEROFBONES];
	float blendWeight_pose_1[MAXNUMBEROFBONES];
	//float blendWeight_pose_2[MAXNUMBEROFBONES];
	//float blendWeight_pose_3[MAXNUMBEROFBONES];
};

#endif
