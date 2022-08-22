/*
    File: GLShader.h
    Shaders utility functions
    Author: Paul peng
    Date: 2022.8.20
*/

#ifndef __GLSHADER_H__
#define __GLSHADER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class GLShader
{
public:
    GLShader();
    GLShader(const std::string vertexPath, const std::string fragmentPath);
    ~GLShader();

    void use();
    void deleteProgram();
    GLuint getProgram();
    void setMat4(const std::string &name, glm::mat4 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setFloat(const std::string &name, float value) const;
    void setInt(const std::string &name, int value) const;
    void setBool(const std::string &name, bool value) const;

private:
    GLuint Program;

    std::string ReadFile(const std::string path);
    GLuint LoadShader(const std::string vertex_path, const std::string fragment_path);
    GLuint CreateShader(const GLchar *vertexCode, const GLchar *fragmentCode);
    void CheckCompileErrors(GLuint shader, std::string type);
};

#endif
