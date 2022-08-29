/*
	File: OpenGL.h
	OpenGL utility class
	Author: Paul peng
	Date: 2022.8.28
*/

#ifndef __OPENGL_H__
#define __OPENGL_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

#define deg2rad(x) ((x) * ((3.1415926f) / (180.0f)))
#define rad2deg(x) ((180.0f) / ((x) * (3.1415926f)))
#define glCheckError() glCheckError_(__FILE__, __LINE__)

GLenum glCheckError_(const char *file, int line);
void error_callback(int error, const char *description);
void resize_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void processInput(GLFWwindow *window);

extern GLFWwindow *window;
extern Camera camera;
extern float deltaTime;
extern float lastX;
extern float lastY;
extern int screen_width;
extern int screen_height;

class OpenGL
{
private:
	// windows settings
	std::string title;
	int width;
	int height;

public:
	// constructors and destructor
	OpenGL();
	OpenGL(int w, int h, std::string str);
	~OpenGL();

	// opengl initialization
	int UseGLFW();
	int UseGLAD();
	int UseIMGUI();
	void InitDefault();
	void DumpInfo();
	void RenderLoop();
};

#endif
