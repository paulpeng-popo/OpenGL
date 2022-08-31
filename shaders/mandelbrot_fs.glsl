#version 460 core

// out vec4 FragColor;

// uniform vec2 resolution;

// void main()
// {
//     vec2 uv = (gl_FragCoord.xy - 0.5 * resolution) / resolution.y;
//     vec3 color = vec3(0.0);

//     color.xy += uv;

//     FragColor = vec4(color, 1.0);
// }

in vec4 gl_FragCoord;
 
out vec4 FragColor;
 
#define MAX_ITERATIONS 500
 
int get_iterations()
{
    float real = ((gl_FragCoord.x / 800.0 - 0.5) ) * 5.0;
    float imag = ((gl_FragCoord.y / 800.0 - 0.5) ) * 5.0;
 
    int iterations = 0;
    float const_real = real;
    float const_imag = imag;
 
    while (iterations < MAX_ITERATIONS)
    {
        float tmp_real = real;
        real = (real * real - imag * imag) + const_real;
        imag = (2.0 * tmp_real * imag) + const_imag;
         
        float dist = real * real + imag * imag;
         
        if (dist > 4.0)
        break;
 
        ++iterations;
    }
    return iterations;
}
 
vec4 return_color()
{
    int iter = get_iterations();
    if (iter == MAX_ITERATIONS)
    {
        gl_FragDepth = 0.0f;
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
 
    float iterations = float(iter) / MAX_ITERATIONS;    
    return vec4(0.0f, iterations, 0.0f, 1.0f);
}
 
void main()
{
    FragColor = return_color();
}
