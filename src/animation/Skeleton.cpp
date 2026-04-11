#include "Skeleton.h"

Skeleton::Skeleton() = default;
Skeleton::~Skeleton() = default;

void Skeleton::AddBone(const std::string& name, int parentIndex, const glm::mat4& inverseBind) 
{
    int idx = m_bones.size();
    m_bones.push_back({name, parentIndex, inverseBind});
    m_nameToIndex[name] = idx;
}

int Skeleton::GetBoneIndex(const std::string& name) const 
{
    auto it = m_nameToIndex.find(name);
    if (it != m_nameToIndex.end()) return it->second;
    return -1;
}

void Skeleton::ComputeFinalMatrices(const std::unordered_map<std::string, glm::mat4>& localTransforms,
                                    std::vector<glm::mat4>& outFinalMatrices) const 
{
    outFinalMatrices.resize(m_bones.size());
    std::vector<glm::mat4> globalTransforms(m_bones.size());

    // 计算全局变换
    for (size_t i = 0; i < m_bones.size(); ++i) 
    {
        const Bone& bone = m_bones[i];
        glm::mat4 local = glm::mat4(1.0f);
        auto it = localTransforms.find(bone.name);
        if (it != localTransforms.end()) local = it->second;
        if (bone.parentIndex >= 0) 
        {
            globalTransforms[i] = globalTransforms[bone.parentIndex] * local;
        } else {
                  globalTransforms[i] = local;
               }
        // 最终矩阵 = 逆绑定矩阵 * 全局变换
        outFinalMatrices[i] = globalTransforms[i] * bone.inverseBindMatrix;
    }
}