#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool is_blur;

const vec2 TexSize = vec2(900, 1500);
const vec2 mosaicSize = vec2(4.0, 4.0);

void main()
{
    vec4 color;

    if (is_blur)
    {
        vec2 intXY = vec2(TexCoords.x*TexSize.x, TexCoords.y*TexSize.y);
        vec2 XYMosaic = vec2(floor(intXY.x/mosaicSize.x)*mosaicSize.x, floor(intXY.y/mosaicSize.y)*mosaicSize.y);
        vec2 UVMosaic = vec2(XYMosaic.x/TexSize.x, XYMosaic.y/TexSize.y);
        color = texture2D(screenTexture, UVMosaic);
    }
    else
    {
        color = texture(screenTexture, TexCoords);
    }

    FragColor = color;
} 
