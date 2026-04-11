#include "AnimationClip.h"
#include <glm/gtc/matrix_transform.hpp>

AnimationClip::AnimationClip(const std::string& name) : m_name(name), m_duration(0.0f) {}
AnimationClip::~AnimationClip() = default;

void AnimationClip::AddChannel(const AnimationChannel& channel) 
{
    m_channels.push_back(channel);
}

void AnimationClip::Update(float time, std::unordered_map<std::string, glm::mat4>& localTransforms) const 
{
    for (const auto& channel : m_channels) 
    {
        glm::vec3 translation = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        if (!channel.timeStamps.empty()) 
        {
            // 找到当前时间的前后关键帧
            size_t idx = 0;
            while (idx + 1 < channel.timeStamps.size() && channel.timeStamps[idx + 1] < time) ++idx;
            float t1 = channel.timeStamps[idx];
            float t2 = (idx + 1 < channel.timeStamps.size()) ? channel.timeStamps[idx + 1] : t1;
            float factor = (t2 > t1) ? (time - t1) / (t2 - t1) : 0.0f;

            // 插值（简单线性插值，四元数用球面线性插值）
            if (!channel.translations.empty()) 
            {
                glm::vec3 p1 = channel.translations[idx];
                glm::vec3 p2 = (idx + 1 < channel.translations.size()) ? channel.translations[idx + 1] : p1;
                translation = glm::mix(p1, p2, factor);
            }
            if (!channel.rotations.empty()) 
            {
                glm::quat r1 = channel.rotations[idx];
                glm::quat r2 = (idx + 1 < channel.rotations.size()) ? channel.rotations[idx + 1] : r1;
                rotation = glm::slerp(r1, r2, factor);
            }
            if (!channel.scales.empty()) 
            {
                glm::vec3 s1 = channel.scales[idx];
                glm::vec3 s2 = (idx + 1 < channel.scales.size()) ? channel.scales[idx + 1] : s1;
                scale = glm::mix(s1, s2, factor);
            }
        }

        glm::mat4 local = glm::translate(glm::mat4(1.0f), translation) *
                          glm::mat4_cast(rotation) *
                          glm::scale(glm::mat4(1.0f), scale);
        localTransforms[channel.nodeName] = local;
    }
}