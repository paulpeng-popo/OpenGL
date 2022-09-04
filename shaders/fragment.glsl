#version 460 core

in vec3 fNormal;
in vec3 fPosition;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light1;
uniform Light light2;
uniform Light light3;

uniform vec3 cameraPosition;
uniform bool grayScale;

out vec4 FragColor;

vec3 CalcDirLight(Light light, Material material, vec3 normal, vec3 viewDir);

void main ()
{
    vec3 norm = normalize(fNormal);
    vec3 viewDir = normalize(cameraPosition - fPosition);

	vec3 result = CalcDirLight(light1, material, norm, viewDir);
	result += CalcDirLight(light2, material, norm, viewDir);
	result += CalcDirLight(light3, material, norm, viewDir);
	
	if (grayScale) {
		float gray = dot(result, vec3(0.299, 0.587, 0.114));
		result = vec3(gray);
	}

	FragColor = vec4(result, 1.0);

}

vec3 CalcDirLight(Light light, Material material, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}
