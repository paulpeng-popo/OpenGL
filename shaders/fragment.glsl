#version 460 core

in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fPosition;
in vec3 cameraPosition;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

uniform Material material;
uniform Light light;
uniform sampler2D texture_diffuse1;
uniform bool lightswitch = true;

out vec4 FragColor;

void main ()
{
	// ambientColor
	vec3 ambientColor = material.ambient * light.ambient;

	// diffuseColor
	vec3 normal = normalize(fNormal);
	vec3 lightDirection = normalize(light.position - fPosition);
	float nDotL = max(dot(normal, lightDirection), 0.0);
	vec3 diffuseColor = material.diffuse * light.diffuse * nDotL;

	// specularColor
	vec3 viewDirection = normalize(cameraPosition - fPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
	vec3 specularColor = material.specular * light.specular * specular;

	// finalColor
	vec3 finalColor;

	if (lightswitch) {
		finalColor = ambientColor + diffuseColor + specularColor;
	} else {
		finalColor = ambientColor + diffuseColor;
	}

	FragColor = vec4(finalColor, 1.0) + texture(texture_diffuse1, fTexCoord);
}
