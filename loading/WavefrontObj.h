#ifndef WAVEFRONTOBJ_H
#define WAVEFRONTOBJ_H

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


#include <vector>
#include <string>



namespace WavefrontObj
{
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UV;

        bool operator==(const Vertex& other) const {
            return Position == other.Position && Normal == other.Normal && UV == other.UV;
        }
    };

    bool LoadObj(const std::string &filename, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);
}


template<>
struct std::hash<WavefrontObj::Vertex>
{
  size_t operator()(WavefrontObj::Vertex const &vertex) const noexcept
  {
    return (hash<glm::vec3>()(vertex.Position) ^ hash<glm::vec3>()(vertex.Normal) << 1) >> 1 ^
           hash<glm::vec2>()(vertex.UV) << 1;
  }
};

#endif //WAVEFRONTOBJ_H