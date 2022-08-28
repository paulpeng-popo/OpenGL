#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>

struct MyTraits : OpenMesh::DefaultTraits
{
    // Let Point and Normal be a vector of doubles
    typedef OpenMesh::Vec3d Point;
    typedef OpenMesh::Vec3d Normal;
    // Already defined in OpenMesh::DefaultTraits
    // HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

    // Uncomment next line to disable attribute PrevHalfedge
    // HalfedgeAttributes( OpenMesh::Attributes::None );
    //
    // or
    //
    // HalfedgeAttributes( 0 );
};

typedef OpenMesh::PolyMesh_ArrayKernelT<> PolyMesh;
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;

class MeshLoader
{
private:
    GLuint VAO;
    GLuint VBO[2];

    void setupMesh();

public:
    TriMesh mesh;
    std::vector<TriMesh::Point> vertices;
    std::vector<TriMesh::Normal> normals;

    MeshLoader();
    ~MeshLoader();
    void loadMesh(std::string file);
    void paintMesh();
};
