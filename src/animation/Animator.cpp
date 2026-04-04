#include "Animator.h"
#include "AnimationClip.h"
#include "Skeleton.h"
#include "scene/Entity.h"
#include "scene/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

Animator::Animator() : m_currentTime(0.0f), m_speed(1.0f), m_playing(false) {}
Animator::~Animator() = default;

void Animator::SetAnimationClip(std::shared_ptr<AnimationClip> clip) {
    m_clip = clip;
}

void Animator::SetNodeMap(const std::unordered_map<std::string, Entity*>& nodeMap) {
    m_nodeMap = nodeMap;
}

void Animator::SetSkeleton(std::shared_ptr<Skeleton> skeleton) {
    m_skeleton = skeleton;
    if (m_skeleton) {
        m_boneMatrices.resize(m_skeleton->GetBones().size());
    }
}

void Animator::Play(float speed) {
    m_speed = speed;
    m_playing = true;
    m_currentTime = 0.0f;
}

void Animator::Stop() {
    m_playing = false;
}

void Animator::Update(float deltaTime) {
    if (!m_playing || !m_clip) return;

    m_currentTime += deltaTime * m_speed;
    float duration = m_clip->GetDuration();
    if (duration > 0.0f && m_currentTime > duration) {
        m_currentTime = fmod(m_currentTime, duration);
    }

    std::unordered_map<std::string, glm::mat4> localTransforms;
    m_clip->Update(m_currentTime, localTransforms);

    // 应用节点变换（非骨骼动画）
    for (auto& pair : localTransforms) {
        const std::string& nodeName = pair.first;
        const glm::mat4& localMatrix = pair.second;
        auto it = m_nodeMap.find(nodeName);
        if (it != m_nodeMap.end()) {
            Entity* entity = it->second;
            if (entity) {
                // 从矩阵中提取位置、旋转、缩放
                glm::vec3 position = localMatrix[3];
                glm::vec3 scale(glm::length(localMatrix[0]), glm::length(localMatrix[1]), glm::length(localMatrix[2]));
                glm::mat3 rotMat3(localMatrix[0] / scale.x, localMatrix[1] / scale.y, localMatrix[2] / scale.z);
                glm::quat rotation = glm::quat_cast(rotMat3);
                Transform* transform = entity->GetTransform();
                transform->SetPosition(position);
                transform->SetRotation(rotation);
                transform->SetScale(scale);
            }
        }
    }

    // 骨骼动画（如果有骨骼）
    if (m_skeleton) {
        m_skeleton->ComputeFinalMatrices(localTransforms, m_boneMatrices);
    }
}