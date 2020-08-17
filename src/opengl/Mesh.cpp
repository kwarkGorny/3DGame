#include "Mesh.hpp"

#include "basic/Logger.hpp"
#include "GLDebug.hpp"

#include <unordered_map>
#include <numeric>
#include <utility>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

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

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();

    std::unordered_map<Vertex, unsigned int> uniqueVertices{};
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
                    attrib.texcoords[2 * index.texcoord_index + 1]
                }
            };

            //const auto pair = uniqueVertices.try_emplace(vertex, static_cast<unsigned int>(vertices.size()));
            //if (pair.second)
            {
                vertices.push_back(vertex);
                minX = std::min(minX, vertex.position.x);
                maxX = std::max(maxX, vertex.position.x);
                minY = std::min(minY, vertex.position.y);
                maxY = std::max(maxY, vertex.position.y);
                minZ = std::min(minZ, vertex.position.z);
                maxZ = std::max(maxZ, vertex.position.z);
            }
            indices.push_back(vertices.size() - 1);
        }
    }
    width = maxX - minX;
    height = maxY - minY;
    depth = maxZ - minZ;
}

Mesh::Mesh(const MeshCPU& meshCPU)
    : indicesCount(meshCPU.indices.size())
    , vertexCount(meshCPU.vertices.size())
    , width(meshCPU.width)
    , height(meshCPU.height)
    , depth(meshCPU.depth)
{
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), meshCPU.vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glGenBuffers(1, &indicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), meshCPU.indices.data(), GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &indicesId);
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteVertexArrays(1, &vertexArrayId);
}

void Mesh::draw()const
{
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    //glDrawArrays(GL_TRIANGLES, 0, vertexCount);

}

void Mesh::bind()const
{
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
}
