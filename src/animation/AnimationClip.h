#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct AnimationChannel {
    std::string nodeName;      // 骨骼或节点名称
    std::vector<float> timeStamps;
    std::vector<glm::vec3> translations;
    std::vector<glm::quat> rotations;
    std::vector<glm::vec3> scales;
};

class AnimationClip {
public:
    AnimationClip(const std::string& name);
    ~AnimationClip();

    void AddChannel(const AnimationChannel& channel);
    void Update(float time, std::unordered_map<std::string, glm::mat4>& localTransforms) const;

    float GetDuration() const { return m_duration; }
    void SetDuration(float duration) { m_duration = duration; }

private:
    std::string m_name;
    float m_duration;
    std::vector<AnimationChannel> m_channels;
};