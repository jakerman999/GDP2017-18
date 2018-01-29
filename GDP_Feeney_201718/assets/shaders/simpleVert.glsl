// Vertex shader
#version 420

//uniform mat4 MVP;
//uniform mat4 mModelRotationOnly;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat4 mModel;
uniform mat4 mWorldInvTranspose;

uniform bool isASkyBox;		// Same as the one in the fragments

uniform sampler2D texHeightMap;			// Is Texture Unit 0 no matter WHAT! WHY???
uniform bool bIsHeightMap;


// "Vertex" attribute (now use 'in')
in vec4 vCol;		// attribute
in vec3 vPos;		// was: vec2 vPos
in vec3 vNorm;		// Vertex normal
in vec4 uvX2;		// Added: UV 1 and 2


out vec4 color;				// was: vec4
out vec3 vertNormal;		// Also in "world" (no view or projection)
out vec3 vecWorldPosition;	// 
out vec4 uvX2out;			// Added: UV 1 and 2 to fragment

void main()
{
    //gl_Position = MVP * vec4(vPos, 0.0, 1.0);	
	vec3 vertPosition = vPos;
	
	mat4 matModel = mModel;
	
//	if ( isASkyBox )
//	{	// DON'T Move the skybox.
//		// Set model matrix to identity
//		// ("moves" the skybox to the camera)
//		// And set the scale value, too
//		matModel = mat4(1.0f);	
//	}
	
	if ( bIsHeightMap )
	{
		// Alter the height of the mesh using a texture...
		vec4 heightChangeXYZ = texture( texHeightMap, uvX2.xy );
		// The RGB is from black to white, so 000 to 111, so I'll ignore 
		//	the the GB and just use the R
		
		vertPosition.y += heightChangeXYZ.r * 1000.0f;
	}
	
	// Calculate the model view projection matrix here
	mat4 MVP = mProjection * mView * matModel;
	gl_Position = MVP * vec4(vertPosition, 1.0f);
	
	// Calculate vertex and normal based on ONLY world 
	vecWorldPosition = vec3( matModel * vec4(vertPosition, 1.0f) ).xyz;
	
	// Inv Tran - strips translation and scale from model transform
	// Alternative is you pass a "rotation only" model mat4
//	mat4 mWorldInTranspose = inverse(transpose(matModel));		/*Now passed in*/
	
	// Was: MVP * vNorm;
	// This normal is in "world space" but only has rotation
	vertNormal = vec3( mWorldInvTranspose * vec4(vNorm, 1.0f) ).xyz;	

//	mat4 mWorldInverseTranspose = inverse(transpose(matModel));
//	vertNormal = vec3( mWorldInverseTranspose * vec4(vNorm, 1.0f) ).xyz;		
	
	
    color = vCol;
	uvX2out = uvX2;			// Sent to fragment shader
}


//	// Or could do this:
//	mWorldInTranpose = inverse(transpose(MVP));
//	
//	// Calculate vertex and normal based on ONLY world 
//	vecWorldPosition = mWorldInTranpose * vec4(position, 1.0f);
//	
//	// Not correct, but for now, just pass along
//	vertNormal = mWorldInTranpose * vNorm;		// Was: MVP * vNorm;

