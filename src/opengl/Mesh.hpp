#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>

#include "Material.hpp"

struct Vertex 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && texCoords == other.texCoords;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const
        {
            return std::hash<glm::vec3>()(vertex.position) ^
                (std::hash<glm::vec2>()(vertex.texCoords) << 1);
        }
    };
}

struct MeshCPU
{
    explicit MeshCPU(const std::string& objFilePath, const std::string& mtlDirPath = "");
    MeshCPU(const std::vector<Vertex>& v, const std::vector<unsigned int>& i) : vertices{v}, indices{i} {}

    unsigned int createIndexBuffer()const;
    unsigned int createVertexBuffer()const;
    void setupVertexArray(unsigned int vertexArrayId, unsigned int vertexBufferId)const;


    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //Material material;
};

struct Mesh
{
    explicit Mesh(const MeshCPU& meshCPU);

    ~Mesh();

    void bind()const;

    const unsigned int vertexArrayId;
    const unsigned int vertexBufferId;
    const unsigned int indicesId;
    const unsigned int indicesCount;
    //Material material;
};

using SharedMesh = std::shared_ptr<Mesh>;