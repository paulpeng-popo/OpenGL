/*
	File: OpenGL.h
	Custom header utilities
	Author: Paul peng
	Date: 2022.8.15
*/

#ifndef __OPENGL_H__
#define __OPENGL_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define deg2rad(x) ((x) * ((3.1415926f) / (180.0f)))
#define rad2deg(x) ((180.0f) / ((x) * (3.1415926f)))
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
GLenum glCheckError_(const char *file, int line);
void error_callback(int error, const char *description);
bool DoTheImportThing(const std::string &pFile);

class OpenGL
{
private:
	// windows settings
	int width;
	int height;
	std::string window_name;

public:
	// constructors and destructor
	OpenGL();
	OpenGL(int width, int height, std::string title);
	~OpenGL();

	// glutInitFuncs
	int UseGLFW();
	int UseGLAD();
	void InitDefault();
	void DumpInfo();
	void RenderLoop();
};

#endif
