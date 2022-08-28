/*
    File: GLShader.cpp
    Shaders Loader Implementation
    Author: Paul peng
    Date: 2022.8.28
*/

#include "../myheaders/GLShader.h"
#include "../myheaders/Error_Debug.h"

GLShader::GLShader() {}

GLShader::GLShader(const std::string vertexPath, const std::string fragmentPath)
{
    Program = LoadShader(vertexPath, fragmentPath);
}

GLShader::~GLShader() {}

void GLShader::use()
{
    glUseProgram(Program);
}

void GLShader::del()
{
    glDeleteProgram(Program);
}

std::string GLShader::ReadFile(const std::string path)
{
    std::string content;
    std::ifstream fileStream(path, std::ios::in);

    if (!fileStream.is_open())
    {
        ERROR("Failed to read file: " + path);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint GLShader::LoadShader(const std::string vertex_path, const std::string fragment_path)
{
    std::string vertexCode = ReadFile(vertex_path);
    std::string fragmentCode = ReadFile(fragment_path);
    return CreateShader(vertexCode.c_str(), fragmentCode.c_str());
}

GLuint GLShader::CreateShader(const GLchar *vertexCode, const GLchar *fragmentCode)
{
    GLuint program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    DEBUG("Compiling vertex shader");

    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    CheckCompileErrors(vertexShader, "VERTEX");

    DEBUG("Compiling fragment shader");

    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    CheckCompileErrors(fragmentShader, "FRAGMENT");

    DEBUG("Linking program");

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    CheckCompileErrors(program, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void GLShader::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint result = GL_FALSE;
    int logLength;

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> ShaderError((logLength > 1) ? logLength : 1);
        glGetShaderInfoLog(shader, logLength, NULL, &ShaderError[0]);
        RAW_DEBUG("%s", &ShaderError[0]);
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &result);
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> programError((logLength > 1) ? logLength : 1);
        glGetProgramInfoLog(shader, logLength, NULL, &programError[0]);
        RAW_DEBUG("%s", &programError[0]);
    }
}

void GLShader::setMat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void GLShader::setVec3(const std::string &name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]);
}

void GLShader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(Program, name.c_str()), value);
}

void GLShader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
}

void GLShader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
}
