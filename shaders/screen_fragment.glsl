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

    if (is_blur)
    {
        if (TexCoords.x > box.x && TexCoords.y < -(box.y-1) && TexCoords.x < box.z && TexCoords.y > -(box.w-1))
        {
            color = texture(screenTexture, TexCoords);
        }
        else
        {
            vec2 mosaic = floor(TexCoords * TexSize / mosaicSize) * mosaicSize / TexSize;
            color = texture(screenTexture, mosaic);
        }
        
    }
    else
    {
        color = texture(screenTexture, TexCoords);
    }

    FragColor = color;
} 
