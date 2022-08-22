#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

struct Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
};

uniform Camera camera;
uniform mat4 model;

out vec2 fTexCoord;
out vec3 fNormal;
out vec3 fPosition;
out vec3 cameraPosition;

void main ()
{
	mat4 mvp = camera.projection * camera.view * model;

	fTexCoord = vTexCoord;
	fNormal = mat3(transpose(inverse(model))) * vNormal;
	fPosition = vec3(model * vec4(vPosition, 1.0));
	cameraPosition = camera.position;
	gl_Position = mvp * vec4(vPosition, 1.0);
}
