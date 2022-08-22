/*
    File: ModelLoader.h
    Assimp model loader
    Author: Paul peng
    Date: 2022.8.20
*/

#ifndef __MODEL_LOADER_H__
#define __MODEL_LOADER_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "GLShader.h"

GLuint TextureFromFile(const char *path, const std::string &directory, GLboolean gamma = false);

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    GLuint id;
    std::string type;
    std::string path;
};

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Mesh
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setupMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Material material;

    Mesh(
        std::vector<Vertex> vertices,
        std::vector<GLuint> indices,
        std::vector<Texture> textures,
        Material material);
    void Draw(GLShader &shader);
    ~Mesh();
};

class ModelLoader
{
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    ModelLoader();
    ModelLoader(std::string const &path, bool gamma = false);
    void Draw(GLShader &shader);
    ~ModelLoader();

private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
