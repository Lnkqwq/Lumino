#pragma once
#include <string>
#include <vector>
#include "Engine/glm/glm.hpp"

// 前置声明，不包含头文件
namespace Lumino {
    struct Animation;
    struct AnimationChannel;
}

namespace Lumino {

struct GLTFMesh {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    unsigned int textureID = 0;
    std::string name;
};

struct GLTFLoadResult {
    std::vector<GLTFMesh> meshes;
    std::vector<Animation> animations;  // 前置声明，OK
    std::vector<std::string> boneNames;
};

class GLTFLoader {
public:
    static GLTFLoadResult Load(const std::string& path);
};

}