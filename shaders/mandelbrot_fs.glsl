#version 460 core

in vec4 gl_FragCoord;

out vec4 FragColor;

uniform vec2 resolution;
uniform float time;

float mandelbrot(vec2 uv, int max_iter)
{
    vec2 c = 4.0 * uv - vec2(0.7, 0.0);
    c = c / pow(time, 7.0) - vec2(0.65, 0.45);
    vec2 z = vec2(0.0, 0.0);

    for (int i = 0; i < max_iter; i++)
    {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > 4.0)
        {
            return float(i) / float(max_iter);
        }
    }
    return 0.0;
}

vec3 hash13(float n)
{
    vec3 p = vec3(0.0, 0.0, 0.0);
    p.x = fract(sin(n) * 5625.246);
    p.y = fract(sin(n+p.x) * 2216.486);
    p.z = fract(sin(p.x+p.y) * 8276.352);
    return p;
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.y;
    vec3 color = vec3(0.0);

    color += hash13(mandelbrot(uv, 4096));

    FragColor = vec4(color, 1.0);
}
