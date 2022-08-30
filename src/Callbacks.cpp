/*
    File: Callbacks.cpp
    Callbacks implementation
    Author: Paul peng
    Date: 2022.8.28
*/

#include "../myheaders/OpenGL.h"

float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;

float clickedX = 0.0f;
float clickedY = 0.0f;

float transformX = 0.0f;
float transformY = 0.0f;

bool mouse_click = false;
bool transform_mode = false;

void resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (zoomMode)
    {
        camera.ProcessMouseScroll(yoffset);
    }
}

void Decompose(glm::mat4 m, glm::mat4 &t, glm::mat4 &r)
{
    glm::mat4 I4 = glm::mat4(1.0f); // Identity
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            if (i == 3 || j == 3)
            {
                r[i][j] = I4[i][j];
                t[i][j] = m[i][j];
            }
            else
            {
                r[i][j] = m[i][j];
                t[i][j] = I4[i][j];
            }
        }
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    lastX = xpos;
    lastY = ypos;

    if (mouse_click)
    {
        float xoffset = xpos - clickedX;
        float yoffset = clickedY - ypos;

        if (yoffset < 0)
            camera.ProcessMouseMove(UPWARD, abs(yoffset));
        if (yoffset > 0)
            camera.ProcessMouseMove(DOWNWARD, abs(yoffset));
        if (xoffset > 0)
            camera.ProcessMouseMove(LEFT, abs(xoffset));
        if (xoffset < 0)
            camera.ProcessMouseMove(RIGHT, abs(xoffset));

        clickedX = lastX;
        clickedY = lastY;
    }

    if (transform_mode)
    {
        float xoffset = xpos - transformX;
        float yoffset = (ypos - transformY);

        glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 viewT, viewR;
        Decompose(view, viewT, viewR);

        glm::mat4 modelT, modelR;
        Decompose(model, modelT, modelR);

        model = modelT * glm::inverse(viewR) * rotX * rotY * viewR * modelR;

        transformX = xpos;
        transformY = ypos;
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        mouse_click = true;
        clickedX = lastX;
        clickedY = lastY;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        mouse_click = false;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        transform_mode = true;
        transformX = lastX;
        transformY = lastY;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        transform_mode = false;
    }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void error_callback(int error, const char *description)
{
    fprintf(stderr, "[Error] %s\n", description);
}

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
