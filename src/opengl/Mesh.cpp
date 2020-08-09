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

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
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
                    1 - attrib.texcoords[2 * index.texcoord_index + 1]
                }
            };

            //const auto pair = uniqueVertices.try_emplace(vertex, static_cast<unsigned int>(vertices.size()));
            //if (pair.second)
            //{
                vertices.push_back(vertex);
            //}
            indices.push_back(vertices.size() - 1);
        }
    }

    for (const auto& material : materials)
    {
        logger::info(material.name);
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
    GLuint bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
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
    , verticesCount(meshCPU.vertices.size())
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
