#pragma once
#include <vector>
#include <string>
#include "Engine/glm/glm.hpp"
#include "Engine/glm/gtc/quaternion.hpp"

namespace Lumino {

struct AnimationChannel {
    std::string boneName;
    std::vector<float> inputTimes;           // 时间点（秒）
    std::vector<glm::quat> rotations;        // 旋转
    std::vector<glm::vec3> translations;     // 位移
    std::vector<glm::vec3> scales;           // 缩放
};

struct Animation {
    std::string name;
    float duration;
    std::vector<AnimationChannel> channels;
};

class AnimationController {
public:
    AnimationController();
    ~AnimationController();
    
    std::vector<std::string> GetAnimationNames() const;

    void AddAnimation(const Animation& anim);
    void Play(const std::string& name, bool loop = true);
    void Update(float deltaTime);
    void Stop();
    
    const std::vector<glm::mat4>& GetBoneMatrices() const { return m_boneMatrices; }
    bool IsPlaying() const { return m_isPlaying; }
    std::string GetCurrentAnimationName() const { return m_currentAnimName; }
    
    void SetBoneNames(const std::vector<std::string>& boneNames);
    
private:
    void ComputeBoneMatrices();
    glm::mat4 InterpolateTranslation(const AnimationChannel& channel, float time);
    glm::quat InterpolateRotation(const AnimationChannel& channel, float time);
    glm::mat4 InterpolateScale(const AnimationChannel& channel, float time);
    
    std::vector<Animation> m_animations;
    const Animation* m_currentAnim = nullptr;
    float m_currentTime = 0.0f;
    bool m_loop = true;
    bool m_isPlaying = false;
    std::string m_currentAnimName;
    
    std::vector<std::string> m_boneNames;
    std::vector<glm::mat4> m_boneMatrices;
};

}