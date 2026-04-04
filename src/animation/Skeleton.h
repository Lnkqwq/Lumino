#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

class Skeleton {
public:
    Skeleton();
    ~Skeleton();

    struct Bone {
        std::string name;
        int parentIndex;
        glm::mat4 inverseBindMatrix;
    };

    void AddBone(const std::string& name, int parentIndex, const glm::mat4& inverseBind);
    int GetBoneIndex(const std::string& name) const;
    const std::vector<Bone>& GetBones() const { return m_bones; }
    void ComputeFinalMatrices(const std::unordered_map<std::string, glm::mat4>& localTransforms,
                              std::vector<glm::mat4>& outFinalMatrices) const;

private:
    std::vector<Bone> m_bones;
    std::unordered_map<std::string, int> m_nameToIndex;
};