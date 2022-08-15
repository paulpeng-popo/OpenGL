/*
	File: main.cpp
	Main logic flow of robot program
	Author: Paul peng
	Date: 2022.8.12
*/

#include "myheaders/OpenGL.h"

const int WIDTH = 800;
const int HEIGHT = 800;
char title[] = "Robot";

int main()
{
    OpenGL opengl = OpenGL(WIDTH, HEIGHT, title);
	if (opengl.UseGLFW() == -1) exit(EXIT_FAILURE); // load GLFW
    if (opengl.UseGLAD() == -1) exit(EXIT_FAILURE); // load GLAD
    opengl.InitDefault(); // initialize basic buffers
    opengl.DumpInfo(); // show some useful version informations
    opengl.RenderLoop(); // render my window in while loop

    glfwTerminate();
    return 0;
}
