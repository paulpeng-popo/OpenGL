#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool is_blur;
uniform vec4 box;

const vec2 TexSize = vec2(400, 400);
const vec2 mosaicSize = vec2(4.0, 4.0);

void main()
{
    vec4 color;

    color = texture(screenTexture, TexCoords);

    if (is_blur)
    {
        if (TexCoords.x > box.x && TexCoords.x < box.z && TexCoords.y > box.y && TexCoords.y < box.w)
        {
            vec2 mosaic = floor(TexCoords * TexSize / mosaicSize) * mosaicSize / TexSize;
            color = texture(screenTexture, mosaic);
        }
    }

    FragColor = color;
} 
