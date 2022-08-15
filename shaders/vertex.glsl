#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vtCoord;

out vec3 fColor;
out vec2 ftCoord;

void main ()
{
	gl_Position = vec4( vPosition, 1.0 );
	fColor = vColor;
	ftCoord = vtCoord;
}
