/*
    File: main.cpp
    Main logic flow of robot program
    Author: Paul peng
    Date: 2022.8.12
*/

#include "../myheaders/OpenGL.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;
char title[] = "Robot";

int main()
{
    OpenGL opengl = OpenGL(WIDTH, HEIGHT, title);
    if (opengl.UseGLFW() == -1)
        exit(EXIT_FAILURE); // load GLFW
    if (opengl.UseGLAD() == -1)
        exit(EXIT_FAILURE); // load GLAD
    if (opengl.UseIMGUI() == -1)
        exit(EXIT_FAILURE); // load IMGUI
    opengl.InitDefault();   // initialize basic buffers
    opengl.DumpInfo();      // show some useful version informations
    opengl.RenderLoop();    // render my window in while loop

    glfwTerminate();
    return 0;
}
