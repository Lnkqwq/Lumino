#pragma once
#include "scene/Component.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class AnimationClip;
class Skeleton;

class Animator : public Component {
public:
    Animator();
    ~Animator();

    void SetAnimationClip(std::shared_ptr<AnimationClip> clip);
    void SetNodeMap(const std::unordered_map<std::string, Entity*>& nodeMap);
    void SetSkeleton(std::shared_ptr<Skeleton> skeleton);
    void Play(float speed = 1.0f);
    void Stop();
    void Update(float deltaTime) override;

    bool IsPlaying() const { return m_playing; }
    std::shared_ptr<AnimationClip> GetAnimationClip() const { return m_clip; }
    const std::vector<glm::mat4>& GetBoneMatrices() const { return m_boneMatrices; }

private:
    std::shared_ptr<AnimationClip> m_clip;
    std::shared_ptr<Skeleton> m_skeleton;
    std::unordered_map<std::string, Entity*> m_nodeMap;
    float m_currentTime;
    float m_speed;
    bool m_playing;
    std::vector<glm::mat4> m_boneMatrices;
};