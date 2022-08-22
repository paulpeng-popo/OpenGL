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

uniform vec3 size;
uniform vec3 rotation;
uniform vec3 translation;

out vec2 fTexCoord;
out vec3 fNormal;
out vec3 fPosition;
out vec3 cameraPosition;

mat4 scale(vec3 s);
mat4 rotate(float angle, float x, float y, float z);
mat4 translate(vec3 t);

#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))
#define rad2deg(x) ((180.0f) / ((x)*(3.1415926f)))

void main ()
{
	mat4 scaleMatrix = scale(size);
	mat4 rotationMatrix = rotate(rotation.x, 1.0, 0.0, 0.0) * rotate(rotation.y, 0.0, 1.0, 0.0) * rotate(rotation.z, 0.0, 0.0, 1.0);
	mat4 translationMatrix = translate(translation);

	mat4 model = translationMatrix * rotationMatrix * scaleMatrix;
	mat4 mvp = camera.projection * camera.view * model;

	fTexCoord = vTexCoord;
	fNormal = mat3(transpose(inverse(model))) * vNormal;
	fPosition = vec3(model * vec4(vPosition, 1.0));
	cameraPosition = camera.position;
	gl_Position = mvp * vec4(vPosition, 1.0);
}

mat4 scale(vec3 s)
{
	return mat4(
		s.x, 0, 0, 0,
		0, s.y, 0, 0,
		0, 0, s.z, 0,
		0, 0, 0, 1
	);
}

mat4 translate(vec3 t)
{
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		t.x, t.y, t.z, 1
	);
}

mat4 rotate(float angle, float x, float y, float z)
{
	float r = deg2rad(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r)) * x * x, (1 - cos(r)) * y * x + sin(r) * z, (1 - cos(r)) * z * x - sin(r) * y, 0);
	vec4 c2 = vec4((1 - cos(r)) * y * x - sin(r) * z, cos(r) + (1 - cos(r)) * y * y, (1 - cos(r)) * z * y + sin(r) * x, 0);
	vec4 c3 = vec4((1 - cos(r)) * z * x + sin(r) * y, (1 - cos(r)) * z * y - sin(r) * x, cos(r) + (1 - cos(r)) * z * z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}
