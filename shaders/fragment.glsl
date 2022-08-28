#version 460 core

in vec3 fNormal;
in vec3 fPosition;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

uniform Light light;

out vec4 FragColor;

void main ()
{
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(light.position - fPosition);
	vec3 viewDir = normalize(-fPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float diff = max(dot(lightDir, normal), 0.0);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 16.0);

	vec3 ambient = light.ambient * 0.3;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec;

	vec3 color = ambient + diffuse + specular;
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
