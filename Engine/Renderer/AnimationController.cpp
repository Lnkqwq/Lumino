#define GLM_ENABLE_EXPERIMENTAL
#include "AnimationController.h"
#include "Engine/glm/gtc/matrix_transform.hpp"
#include "Engine/glm/gtc/quaternion.hpp"
#include "Engine/glm/gtx/quaternion.hpp"
#include <algorithm>
#include <iostream>

namespace Lumino {

AnimationController::AnimationController() {}

AnimationController::~AnimationController() {}

void AnimationController::AddAnimation(const Animation& anim) {
    m_animations.push_back(anim);
}

void AnimationController::SetBoneNames(const std::vector<std::string>& boneNames) {
    m_boneNames = boneNames;
    m_boneMatrices.resize(boneNames.size(), glm::mat4(1.0f));
}

void AnimationController::Play(const std::string& name, bool loop) {
    for (const auto& anim : m_animations) {
        if (anim.name == name) {
            m_currentAnim = &anim;
            m_currentTime = 0.0f;
            m_loop = loop;
            m_isPlaying = true;
            m_currentAnimName = name;
            std::cout << "Playing animation: " << name << std::endl;
            return;
        }
    }
    std::cerr << "Animation not found: " << name << std::endl;
}

void AnimationController::Update(float deltaTime) {
    if (!m_isPlaying || !m_currentAnim) return;
    
    m_currentTime += deltaTime;
    
    if (m_currentTime >= m_currentAnim->duration) {
        if (m_loop) {
            m_currentTime = fmod(m_currentTime, m_currentAnim->duration);
        } else {
            m_currentTime = m_currentAnim->duration;
            m_isPlaying = false;
        }
    }
    
    ComputeBoneMatrices();
}

void AnimationController::Stop() {
    m_isPlaying = false;
    m_currentAnim = nullptr;
}

void AnimationController::ComputeBoneMatrices() {
    if (!m_currentAnim) return;
    
    // 初始化为单位矩阵
    for (auto& mat : m_boneMatrices) {
        mat = glm::mat4(1.0f);
    }
    
    // 遍历所有通道，计算每个骨骼的变换矩阵
    for (const auto& channel : m_currentAnim->channels) {
        // 查找骨骼索引
        int boneIndex = -1;
        for (size_t i = 0; i < m_boneNames.size(); i++) {
            if (m_boneNames[i] == channel.boneName) {
                boneIndex = i;
                break;
            }
        }
        if (boneIndex < 0) continue;
        
        // 计算变换矩阵
        glm::mat4 translation = InterpolateTranslation(channel, m_currentTime);
        
        // 修复：将四元数转换为旋转矩阵
        glm::quat rotationQuat = InterpolateRotation(channel, m_currentTime);
        glm::mat4 rotation = glm::mat4_cast(rotationQuat);  // ← 关键修复
        
        glm::mat4 scale = InterpolateScale(channel, m_currentTime);
        
        // 组合：T * R * S
        m_boneMatrices[boneIndex] = translation * rotation * scale;
    }
}

glm::mat4 AnimationController::InterpolateTranslation(const AnimationChannel& channel, float time) {
    if (channel.inputTimes.empty() || channel.translations.empty()) {
        return glm::mat4(1.0f);
    }
    
    // 找到时间区间
    size_t i = 0;
    for (; i < channel.inputTimes.size() - 1; i++) {
        if (time <= channel.inputTimes[i + 1]) break;
    }
    
    if (i >= channel.inputTimes.size() - 1) {
        return glm::translate(glm::mat4(1.0f), channel.translations.back());
    }
    
    float t1 = channel.inputTimes[i];
    float t2 = channel.inputTimes[i + 1];
    float factor = (time - t1) / (t2 - t1);
    
    glm::vec3 trans = glm::mix(channel.translations[i], channel.translations[i + 1], factor);
    return glm::translate(glm::mat4(1.0f), trans);
}

glm::quat AnimationController::InterpolateRotation(const AnimationChannel& channel, float time) {
    if (channel.inputTimes.empty() || channel.rotations.empty()) {
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }
    
    size_t i = 0;
    for (; i < channel.inputTimes.size() - 1; i++) {
        if (time <= channel.inputTimes[i + 1]) break;
    }
    
    if (i >= channel.inputTimes.size() - 1) {
        return channel.rotations.back();
    }
    
    float t1 = channel.inputTimes[i];
    float t2 = channel.inputTimes[i + 1];
    float factor = (time - t1) / (t2 - t1);
    
    return glm::slerp(channel.rotations[i], channel.rotations[i + 1], factor);
}

glm::mat4 AnimationController::InterpolateScale(const AnimationChannel& channel, float time) {
    if (channel.inputTimes.empty() || channel.scales.empty()) {
        return glm::mat4(1.0f);
    }
    
    size_t i = 0;
    for (; i < channel.inputTimes.size() - 1; i++) {
        if (time <= channel.inputTimes[i + 1]) break;
    }
    
    if (i >= channel.inputTimes.size() - 1) {
        return glm::scale(glm::mat4(1.0f), channel.scales.back());
    }
    
    float t1 = channel.inputTimes[i];
    float t2 = channel.inputTimes[i + 1];
    float factor = (time - t1) / (t2 - t1);
    
    glm::vec3 scale = glm::mix(channel.scales[i], channel.scales[i + 1], factor);
    return glm::scale(glm::mat4(1.0f), scale);
}

std::vector<std::string> AnimationController::GetAnimationNames() const {
    std::vector<std::string> names;
    for (const auto& anim : m_animations) {
        names.push_back(anim.name);
    }
    return names;
}

}