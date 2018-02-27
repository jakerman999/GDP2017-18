// Debug GEOMETRY shader
#version 400 core

layout(triangles) 			in;
layout(triangle_strip) 		out;
layout(max_vertices = 3) 	out;

// gl_in[] array is built in (don't have to declare)
// gl_in[0] is the 1st vertex
// gl_in[1] is the 2nd vertex
// gl_in[2] is the 3rd vertex

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
	
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}



