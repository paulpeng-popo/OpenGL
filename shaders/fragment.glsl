#version 460 core

out vec4 FragColor;

in vec3 fColor;
in vec2 ftCoord;

uniform float alpha;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main ()
{
	FragColor = mix(texture(tex1, ftCoord), texture(tex2, ftCoord), alpha);
}
