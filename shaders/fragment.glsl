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
uniform Light light;
uniform vec3 cameraPosition;
uniform bool grayScale;
uniform bool mosaic;

const float mosaicSize = 16.0;

out vec4 FragColor;

void main ()
{
	// ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // specular
    vec3 viewDir = normalize(cameraPosition - fPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    vec3 result = ambient + diffuse + specular;
	float luminance = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;
	
	FragColor = vec4(result, 1.0);

	if (grayScale) {
		FragColor = vec4(luminance, luminance, luminance, 1.0);
	}

	if (mosaic) {
		FragColor.rgb = FragColor.rgb * (1.0 - (mod(gl_FragCoord.x, mosaicSize) + mod(gl_FragCoord.y, mosaicSize)) / (2.0 * mosaicSize));
	}
}
