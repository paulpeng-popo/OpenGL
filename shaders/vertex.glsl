#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fNormal;
out vec3 fPosition;

void main ()
{
	mat4 mvp = projection * view * model;
	fNormal = mat3(transpose(inverse(model))) * vNormal;
	fPosition = vec3(model * vec4(vPosition, 1.0));
	gl_Position = mvp * vec4(vPosition, 1.0);
}
