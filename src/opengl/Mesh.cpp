#include "Mesh.hpp"

#include "basic/Logger.hpp"
#include "GLDebug.hpp"

#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

namespace
{
    GLuint createVertexArray()
    {
        GLuint bufferId;
        CHECK_GL(glGenVertexArrays(1, &bufferId));
        return bufferId;
    }
    void printTinyObjMaterial(const tinyobj::material_t& material)
    {
        printf("material name = %s\n", material.name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n", material.ambient[0], material.ambient[1], material.ambient[2]);
        printf("  material.Kd = (%f, %f ,%f)\n", material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        printf("  material.Ks = (%f, %f ,%f)\n", material.specular[0], material.specular[1], material.specular[2]);
        printf("  material.Tr = (%f, %f ,%f)\n", material.transmittance[0], material.transmittance[1], material.transmittance[2]);
        printf("  material.Ke = (%f, %f ,%f)\n", material.emission[0], material.emission[1], material.emission[2]);
        printf("  material.Ns = %f\n", material.shininess);
        printf("  material.Ni = %f\n", material.ior);
        printf("  material.dissolve = %f\n", material.dissolve);
        printf("  material.illum = %d\n", material.illum);
        printf("  material.map_Ka = %s\n", material.ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", material.diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", material.specular_texname.c_str());
        printf("  material.map_Ns = %s\n", material.specular_highlight_texname.c_str());
    }
}

MeshCPU::MeshCPU(const std::string& objfilePath, const std::string& mtlDirPath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    const bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objfilePath.c_str(), mtlDirPath.c_str());
    logger::warning(!warn.empty(),"while loading obj there were warnings: {} ", warn);
    logger::error(!err.empty(), "while loading obj there were errors: {} ", err);
    if (!success)
    {
        return;
    }

    std::vector<Material> materialVect;
    materialVect.reserve(materials.size());
    for (const auto& material : materials)
    {
        Material m = materialVect.emplace_back();
        m.ambient = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
        m.diffuse = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        m.specular = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
        //m.transmittance = glm::vec3(material.transmittance[0], material.transmittance[1], material.transmittance[2]);
        //m.emission = glm::vec3(material.emission[0], material.emission[1], material.emission[2]);
        m.shininess = material.shininess;
        //m.ior = material.ior;
        //m.dissolve = material.dissolve;
        //m.illum = material.illum;
        //m.ambientTexture = material.ambient_texname;
        //m.diffuseTexture = material.diffuse_texname;
        //m.specularTexture = material.specular_texname;
        //m.specularHighlightTexture = material.specular_highlight_texname;
        //printTinyObjMaterial(material);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (const auto& shape : shapes)
    {
        auto& mesh = shape.mesh;
        size_t index_offset = 0;
        for (const auto f : mesh.num_face_vertices)
        {
            // Loop over vertices in the face.
            for (size_t v = 0; v < f; v++) 
            {
                // access to vertex
                tinyobj::index_t index = mesh.indices[index_offset + v];
                Vertex vertex{
                    {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    },
                    {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    },
                    {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    }
                };
                const auto pair = uniqueVertices.try_emplace(vertex, static_cast<unsigned int>(vertices.size()));
                if (pair.second)
                {
                   vertices.push_back(vertex);
                }
                indices.push_back(pair.first->second);
            }
            index_offset += f;

            //const auto materialId = shape.mesh.material_ids[f];
            //if(materialId < materialVect.size())
            //{
            //    material = materialVect[shape.mesh.material_ids[f]];
            //}
        }
    }
}

unsigned int MeshCPU::createIndexBuffer()const
{
    using indicesType = typename std::decay<decltype(*indices.begin())>::type;
    GLuint bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indicesType), indices.data(), GL_STATIC_DRAW);
    return bufferId;
}

unsigned int MeshCPU::createVertexBuffer()const
{
    using vertexType = typename std::decay<decltype(*vertices.begin())>::type;
    GLuint bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexType), vertices.data(), GL_STATIC_DRAW);
    return bufferId;
}

void MeshCPU::setupVertexArray(unsigned int vertexArrayId, unsigned int vertexBufferId)const
{
    CHECK_GL(glBindVertexArray(vertexArrayId));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));

    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)));

    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)));

    CHECK_GL(glEnableVertexAttribArray(2));
    CHECK_GL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords)));
}


Mesh::Mesh(const MeshCPU& meshCPU)
    : vertexArrayId(createVertexArray())
    , vertexBufferId(meshCPU.createVertexBuffer())
    , indicesId(meshCPU.createIndexBuffer())
    , indicesCount(meshCPU.indices.size())
//    , material(meshCPU.material)
{
    meshCPU.setupVertexArray(vertexArrayId, vertexBufferId);
}

Mesh::~Mesh()
{
    CHECK_GL(glDeleteBuffers(1, &indicesId));
    CHECK_GL(glDeleteBuffers(1, &vertexBufferId));
    CHECK_GL(glDeleteVertexArrays(1, &vertexArrayId));
}

void Mesh::bind()const
{
    CHECK_GL(glBindVertexArray(vertexArrayId));
    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId));
}
