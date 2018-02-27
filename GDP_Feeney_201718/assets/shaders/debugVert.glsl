// Debug VERTEX shader
#version 420                                   
												
uniform mat4 mModel;    	                         
uniform mat4 mView;                             
uniform mat4 mProjection;                       
											
in vec4 vPosition;                             
in vec4 vColour;                                
												
//out vec4 vertColour;                           
												
void main()                                    
{                                               
	mat4 MVP = mProjection * mView * mModel;    
	gl_Position = MVP * vPosition;              
												
	//vertColour = vColour;                       
}
