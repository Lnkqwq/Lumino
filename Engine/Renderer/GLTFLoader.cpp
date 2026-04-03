#include "GLTFLoader.h"
#include "AnimationController.h"  // 在这里包含完整定义
#include "Engine/Renderer/Renderer.h"

#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#endif

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

#include "Engine/glm/gtc/quaternion.hpp"
#include <iostream>
#include <cstring>

namespace Lumino 
{

GLTFLoadResult GLTFLoader::Load(const std::string& path)
{
    GLTFLoadResult result;
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;
    
    bool ret = false;
    
    // 根据文件扩展名选择加载方式
    if (path.size() > 4 && path.substr(path.size() - 4) == ".glb") {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
    } else {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    }
    
    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    if (!err.empty()) std::cout << "ERR: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load glTF: " << path << std::endl;
        return result;  // 修复1: 返回result而不是meshes
    }
    
    std::cout << "Loaded glTF with " << model.meshes.size() << " meshes" << std::endl;
    
    // 遍历所有网格
    for (size_t i = 0; i < model.meshes.size(); i++) {
        const tinygltf::Mesh& mesh = model.meshes[i];
        
        for (size_t j = 0; j < mesh.primitives.size(); j++) {
            const tinygltf::Primitive& primitive = mesh.primitives[j];
            GLTFMesh gltfMesh;
            gltfMesh.name = mesh.name;
            
            // 获取位置数据
            if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
                const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("POSITION")];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                
                const float* positions = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t v = 0; v < accessor.count; v++) {
                    glm::vec3 pos(positions[v*3], positions[v*3+1], positions[v*3+2]);
                    gltfMesh.positions.push_back(pos);
                }
            }
            
            // 获取法线数据
            if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("NORMAL")];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                
                const float* normals = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t v = 0; v < accessor.count; v++) {
                    glm::vec3 normal(normals[v*3], normals[v*3+1], normals[v*3+2]);
                    gltfMesh.normals.push_back(normal);
                }
            }
            
            // 获取 UV 数据
            if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                
                const float* uvs = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t v = 0; v < accessor.count; v++) {
                    glm::vec2 uv(uvs[v*2], uvs[v*2+1]);
                    gltfMesh.texCoords.push_back(uv);
                }
            }
            
            // 获取索引数据
            if (primitive.indices >= 0) {
                const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                
                if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                    const unsigned int* indices = reinterpret_cast<const unsigned int*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                    for (size_t idx = 0; idx < accessor.count; idx++) {
                        gltfMesh.indices.push_back(indices[idx]);
                    }
                } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                    const unsigned short* indices = reinterpret_cast<const unsigned short*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                    for (size_t idx = 0; idx < accessor.count; idx++) {
                        gltfMesh.indices.push_back(indices[idx]);
                    }
                }
            }
            
            result.meshes.push_back(gltfMesh);  // 修复2: 使用result.meshes
            std::cout << "Mesh " << gltfMesh.name << ": " 
                      << gltfMesh.positions.size() << " vertices, "
                      << gltfMesh.indices.size() / 3 << " triangles" << std::endl;
        }
    }
    
    // 解析骨骼名称 - 修正版
for (const auto& skin : model.skins) {
    // skin.joints 是 std::vector<int>，直接存储骨骼节点的索引
    for (int jointNodeIndex : skin.joints) {
        if (jointNodeIndex >= 0 && jointNodeIndex < (int)model.nodes.size()) {
            const std::string& boneName = model.nodes[jointNodeIndex].name;
            result.boneNames.push_back(boneName);
            std::cout << "Found bone: " << boneName << std::endl;
        }
    }
}
    
    // 解析动画（合并重复代码，只保留一份）
    for (const auto& anim : model.animations) {
        Animation engineAnim;
        engineAnim.name = anim.name;
        
        // 计算 duration
        float maxDuration = 0.0f;
        for (const auto& channel : anim.channels) {
            const auto& sampler = anim.samplers[channel.sampler];
            const auto& inputAccessor = model.accessors[sampler.input];
            const auto& inputBufferView = model.bufferViews[inputAccessor.bufferView];
            const auto& inputBuffer = model.buffers[inputBufferView.buffer];
            const float* times = reinterpret_cast<const float*>(
                &inputBuffer.data[inputBufferView.byteOffset + inputAccessor.byteOffset]);
            if (inputAccessor.count > 0) {
                float lastTime = times[inputAccessor.count - 1];
                if (lastTime > maxDuration) maxDuration = lastTime;
            }
        }
        engineAnim.duration = maxDuration;
        
        // 解析 channels
        for (const auto& channel : anim.channels) {
            const auto& sampler = anim.samplers[channel.sampler];
            const auto& inputAccessor = model.accessors[sampler.input];
            const auto& outputAccessor = model.accessors[sampler.output];
            
            std::string boneName = model.nodes[channel.target_node].name;
            
            AnimationChannel engineChannel;
            engineChannel.boneName = boneName;
            
            // 读取时间轴
            const auto& inputBufferView = model.bufferViews[inputAccessor.bufferView];
            const auto& inputBuffer = model.buffers[inputBufferView.buffer];
            const float* times = reinterpret_cast<const float*>(
                &inputBuffer.data[inputBufferView.byteOffset + inputAccessor.byteOffset]);
            for (size_t i = 0; i < inputAccessor.count; i++) {
                engineChannel.inputTimes.push_back(times[i]);
            }
            
            // 读取变换数据
            const auto& outputBufferView = model.bufferViews[outputAccessor.bufferView];
            const auto& outputBuffer = model.buffers[outputBufferView.buffer];
            
            if (sampler.interpolation == "LINEAR") {
                if (channel.target_path == "rotation") {
                    const float* values = reinterpret_cast<const float*>(
                        &outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset]);
                    for (size_t i = 0; i < outputAccessor.count / 4; i++) {
                        glm::quat q(values[i*4 + 1], values[i*4 + 2], values[i*4 + 3], values[i*4]);
                        engineChannel.rotations.push_back(q);
                    }
                } else if (channel.target_path == "translation") {
                    const float* values = reinterpret_cast<const float*>(
                        &outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset]);
                    for (size_t i = 0; i < outputAccessor.count / 3; i++) {
                        engineChannel.translations.push_back(glm::vec3(values[i*3], values[i*3+1], values[i*3+2]));
                    }
                } else if (channel.target_path == "scale") {
                    const float* values = reinterpret_cast<const float*>(
                        &outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset]);
                    for (size_t i = 0; i < outputAccessor.count / 3; i++) {
                        engineChannel.scales.push_back(glm::vec3(values[i*3], values[i*3+1], values[i*3+2]));
                    }
                }
            }
            
            engineAnim.channels.push_back(engineChannel);
        }
        
        result.animations.push_back(engineAnim);
        std::cout << "Loaded animation: " << engineAnim.name << " (" << engineAnim.channels.size() << " channels)" << std::endl;
    }
    
    return result;
}
}