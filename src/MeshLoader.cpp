#include "../myheaders/MeshLoader.h"
#include "../myheaders/Error_Debug.h"

MeshLoader::MeshLoader() {}

MeshLoader::MeshLoader(std::string file)
{
    loadMesh(file);
}

MeshLoader::~MeshLoader() {}

void MeshLoader::loadMesh(std::string file)
{
    mesh.request_vertex_normals();
    mesh.request_face_normals();

    OpenMesh::IO::Options opt;
    opt += OpenMesh::IO::Options::VertexNormal;
    opt += OpenMesh::IO::Options::FaceNormal;

    if (!OpenMesh::IO::read_mesh(mesh, file, opt))
    {
        ERROR("Could not read mesh from file: %s", file.c_str());
        exit(EXIT_FAILURE);
    }

    mesh.update_normals();

    vertices.clear();
    normals.clear();

    for (auto face : mesh.faces())
    {
        for (auto vertex : mesh.fv_range(face))
        {
            auto point = mesh.point(vertex);
            auto normal = mesh.normal(face);

            vertices.push_back(point);
            normals.push_back(normal);
        }
    }

    std::cout << "Loaded mesh from file: " << file << std::endl;

    setupMesh();

    mesh.release_vertex_normals();
    mesh.release_face_normals();
}

void MeshLoader::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshLoader::paintMesh()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}
