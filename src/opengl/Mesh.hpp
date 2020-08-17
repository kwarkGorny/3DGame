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
    bool operator==(const Vertex& other) const 
    {
        return position == other.position && normal == other.normal && texCoords == other.texCoords;
    }
};

    template<> struct hash<Vertex> {
namespace std 
{
    template<> struct hash<Vertex> 
    {
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

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    float width = 0;
    float height = 0;
    float depth = 0;
};

struct Mesh
{
    explicit Mesh(const MeshCPU& meshCPU);

    ~Mesh();

    void draw()const;

    void bind()const;

    glm::vec3 getSize()const { return { width, height, depth }; }

    unsigned int vertexArrayId;
    unsigned int vertexBufferId;
    unsigned int indicesId;

    unsigned int indicesCount;
    unsigned int vertexCount;

    float width;
    float height;
    float depth;
};

using SharedMesh = std::shared_ptr<Mesh>;