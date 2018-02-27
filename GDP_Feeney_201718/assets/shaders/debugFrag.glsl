// Debug FRAGMENT shader
#version 420 
											
//in vec4 vertColour;                         
											
void main()                                
{                                          
//	gl_FragColor.rgb = vertColour.rgb;     
//	gl_FragColor.a = vertColour.a;      

	gl_FragColor.rgb = vec3(1.0f, 1.0f, 0.0f);
	gl_FragColor.a = 1.0f;
}
