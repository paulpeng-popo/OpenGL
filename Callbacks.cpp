/*
    File: Callbacks.cpp
    Custom header implementation
    Author: Paul peng
    Date: 2022.8.16
*/

#include "myheaders/OpenGL.h"

/*
    Function: Set viewport dynamically fit window size
    Params:
        window: GLFWwindow identifier
        width:  Width after resizing
        height: Height after resizing
    Return:		None
*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
    Function: Use mouse scroll to control camera zoom in and out
    Params:
        window: GLFWwindow identifier
        xoffset:  Scroll x offset
        yoffset: Scroll y offset
    Return:		None
*/
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (cursor_lock)
        camera.ProcessMouseScroll(yoffset);
}

/*
    Function: Use mouse to control camera direction
    Params:
        window: GLFWwindow identifier
        xpos:  Mouse x position
        ypos: Mouse y position
    Return:		None
*/
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (cursor_lock)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

/*
    Function: Use mouse button to control light switch
    Params:
        window: GLFWwindow identifier
        button: Mouse button
        action: Mouse action
        mods: Mouse modifiers
    Return:		None
*/
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        if (cursor_lock)
        {
            cursor_lock = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            cursor_lock = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        // if (lightswitch)
        // {
        //     std::cout << "[INFO] Light is off" << std::endl;
        //     lightswitch = false;
        // }
        // else
        // {
        //     std::cout << "[INFO] Light is on" << std::endl;
        //     lightswitch = true;
        // }
    }
}

/*
    Function: Manipulate keyboard input in glfw window
    Params:
        window: GLFWwindow identifier
    Return:		None
*/
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

/*
    Function: GLFW callback for error checker
    Params:		None
    Return:		None
*/
void error_callback(int error, const char *description)
{
    fprintf(stderr, "[Error] %s\n", description);
}

/*
    Function: My custom error checker
    Params:
        file:	Error located file
        line:	Error located line
    Return:		Error code
*/
GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cerr << "[ERROR] " << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
