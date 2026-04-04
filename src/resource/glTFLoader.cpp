#include "glTFLoader.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/MeshRenderer.h"
#include "components/SkinnedMeshRenderer.h"
#include "animation/Animator.h"
#include "animation/AnimationClip.h"
#include "animation/Skeleton.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/Texture.h"
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>

static std::unordered_map<std::string, Entity*> s_nodeToEntity;

static glm::mat4 ToGLMMat4(const cgltf_float mat[16]) {
    glm::mat4 m;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[j][i] = mat[i * 4 + j];
    return m;
}

static glm::vec3 ToGLMVec3(const cgltf_float vec[3]) {
    return glm::vec3(vec[0], vec[1], vec[2]);
}

static glm::quat ToGLMQuat(const cgltf_float quat[4]) {
    return glm::quat(quat[3], quat[0], quat[1], quat[2]);
}

// 存储节点名称到实体的映射，以便骨骼绑定

static void ProcessNode(cgltf_node* node, Entity* parentEntity, Scene* scene, const glm::mat4& parentTransform = glm::mat4(1.0f)) {
    glm::mat4 localMatrix(1.0f);
    if (node->has_matrix) {
        localMatrix = ToGLMMat4(node->matrix);
    } else {
        glm::vec3 translation(0.0f);
        glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale(1.0f);
        if (node->has_translation) translation = ToGLMVec3(node->translation);
        if (node->has_rotation) rotation = ToGLMQuat(node->rotation);
        if (node->has_scale) scale = ToGLMVec3(node->scale);
        localMatrix = glm::translate(glm::mat4(1.0f), translation) *
                      glm::mat4_cast(rotation) *
                      glm::scale(glm::mat4(1.0f), scale);
    }

    glm::mat4 worldMatrix = parentTransform * localMatrix;

    std::string nodeName = node->name ? node->name : "Node";
    Entity* entity = scene->CreateEntity(nodeName);
    s_nodeToEntity[nodeName] = entity;

    

    // 提取 Transform
    glm::vec3 pos = worldMatrix[3];
    glm::vec3 scale(glm::length(worldMatrix[0]), glm::length(worldMatrix[1]), glm::length(worldMatrix[2]));
    glm::mat3 rotMat3(worldMatrix[0] / scale.x, worldMatrix[1] / scale.y, worldMatrix[2] / scale.z);
    glm::quat rot = glm::quat_cast(rotMat3);
    entity->GetTransform()->SetPosition(pos);
    entity->GetTransform()->SetRotation(rot);
    entity->GetTransform()->SetScale(scale);

    // 处理 Mesh（蒙皮或静态）
    if (node->mesh) {
        for (cgltf_size p = 0; p < node->mesh->primitives_count; ++p) {
            cgltf_primitive* prim = &node->mesh->primitives[p];
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<glm::ivec4> boneIndices;
            std::vector<glm::vec4> boneWeights;

            // 提取顶点数据
            cgltf_accessor* posAccessor = nullptr;
            cgltf_accessor* normalAccessor = nullptr;
            cgltf_accessor* texAccessor = nullptr;
            cgltf_accessor* jointsAccessor = nullptr;
            cgltf_accessor* weightsAccessor = nullptr;
            for (cgltf_size i = 0; i < prim->attributes_count; ++i) {
                if (prim->attributes[i].type == cgltf_attribute_type_position) posAccessor = prim->attributes[i].data;
                else if (prim->attributes[i].type == cgltf_attribute_type_normal) normalAccessor = prim->attributes[i].data;
                else if (prim->attributes[i].type == cgltf_attribute_type_texcoord) texAccessor = prim->attributes[i].data;
                else if (prim->attributes[i].type == cgltf_attribute_type_joints) jointsAccessor = prim->attributes[i].data;
                else if (prim->attributes[i].type == cgltf_attribute_type_weights) weightsAccessor = prim->attributes[i].data;
            }
            cgltf_accessor* indexAccessor = prim->indices;

            if (posAccessor) {
                cgltf_buffer_view* view = posAccessor->buffer_view;
                char* data = (char*)view->buffer->data + view->offset + posAccessor->offset;
                for (cgltf_size i = 0; i < posAccessor->count; ++i) {
                    float* pos = (float*)(data + i * posAccessor->stride);
                    glm::vec3 position(pos[0], pos[1], pos[2]);
                    glm::vec3 normal(0.0f);
                    glm::vec2 tex(0.0f);
                    glm::ivec4 joints(0);
                    glm::vec4 weights(0.0f);
                    if (normalAccessor) {
                        float* norm = (float*)((char*)normalAccessor->buffer_view->buffer->data + normalAccessor->buffer_view->offset + normalAccessor->offset + i * normalAccessor->stride);
                        normal = glm::vec3(norm[0], norm[1], norm[2]);
                    }
                    if (texAccessor) {
                        float* uv = (float*)((char*)texAccessor->buffer_view->buffer->data + texAccessor->buffer_view->offset + texAccessor->offset + i * texAccessor->stride);
                        tex = glm::vec2(uv[0], uv[1]);
                    }
                    if (jointsAccessor) {
                        // 支持 uint8/uint16 类型
                        char* jntData = (char*)jointsAccessor->buffer_view->buffer->data + jointsAccessor->buffer_view->offset + jointsAccessor->offset + i * jointsAccessor->stride;
                        if (jointsAccessor->component_type == cgltf_component_type_r_8u) {
                            uint8_t* j = (uint8_t*)jntData;
                            joints = glm::ivec4(j[0], j[1], j[2], j[3]);
                        } else if (jointsAccessor->component_type == cgltf_component_type_r_16u) {
                            uint16_t* j = (uint16_t*)jntData;
                            joints = glm::ivec4(j[0], j[1], j[2], j[3]);
                        } else {
                            uint32_t* j = (uint32_t*)jntData;
                            joints = glm::ivec4(j[0], j[1], j[2], j[3]);
                        }
                    }
                    if (weightsAccessor) {
                        float* w = (float*)((char*)weightsAccessor->buffer_view->buffer->data + weightsAccessor->buffer_view->offset + weightsAccessor->offset + i * weightsAccessor->stride);
                        weights = glm::vec4(w[0], w[1], w[2], w[3]);
                    }
                    vertices.push_back({position, normal, tex});
                    boneIndices.push_back(joints);
                    boneWeights.push_back(weights);
                }
            }

            if (indexAccessor) {
                cgltf_buffer_view* view = indexAccessor->buffer_view;
                char* data = (char*)view->buffer->data + view->offset + indexAccessor->offset;
                for (cgltf_size i = 0; i < indexAccessor->count; ++i) {
                    unsigned int idx;
                    if (indexAccessor->component_type == cgltf_component_type_r_32u)
                        idx = *(unsigned int*)(data + i * indexAccessor->stride);
                    else if (indexAccessor->component_type == cgltf_component_type_r_16u)
                        idx = *(unsigned short*)(data + i * indexAccessor->stride);
                    else
                        idx = *(unsigned char*)(data + i * indexAccessor->stride);
                    indices.push_back(idx);
                }
            }

// 调试输出
std::cout << "Mesh has " << vertices.size() << " vertices, " << indices.size() << " indices" << std::endl;

            auto mesh = std::make_shared<Mesh>(vertices, indices);
            auto material = std::make_shared<Material>();
            if (prim->material && prim->material->has_pbr_metallic_roughness) {
                cgltf_float* baseColor = prim->material->pbr_metallic_roughness.base_color_factor;
                material->SetBaseColor(glm::vec3(baseColor[0], baseColor[1], baseColor[2]));
                material->SetMetallic(prim->material->pbr_metallic_roughness.metallic_factor);
                material->SetRoughness(prim->material->pbr_metallic_roughness.roughness_factor);
            }

// 检查当前节点是否有蒙皮（skin）
if (node->skin != nullptr) {
    auto skinnedRenderer = entity->AddComponent<SkinnedMeshRenderer>();
    skinnedRenderer->SetMesh(mesh);
    skinnedRenderer->SetMaterial(material);
    // 稍后绑定 Animator
} else {
    auto renderer = entity->AddComponent<MeshRenderer>();
    renderer->SetMesh(mesh);
    renderer->SetMaterial(material);
}
        }
    }

    // 处理子节点
    for (cgltf_size i = 0; i < node->children_count; ++i) {
        ProcessNode(node->children[i], entity, scene, worldMatrix);
    }
}

Entity* glTFLoader::Load(const std::string& path, Scene* scene) {
std::cout << "glTFLoader::Load: " << path << std::endl;

    s_nodeToEntity.clear();

    cgltf_options options = {};
    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);
    if (result != cgltf_result_success) {
        std::cerr << "Failed to parse glTF: " << path << std::endl;
        return nullptr;
    }
std::cout << "cgltf parse success." << std::endl;

    result = cgltf_load_buffers(&options, data, path.c_str());
    if (result != cgltf_result_success) {
        std::cerr << "Failed to load buffers" << std::endl;
        cgltf_free(data);
        return nullptr;
    }

    std::cout << "Loaded " << data->meshes_count << " meshes, " << data->skins_count << " skins, " << data->animations_count << " animations." << std::endl;

    Entity* rootEntity = scene->CreateEntity("glTF_Root");
    if (data->scene) {
        for (cgltf_size i = 0; i < data->scene->nodes_count; ++i) {
            ProcessNode(data->scene->nodes[i], rootEntity, scene);
        }
    } else if (data->scenes_count > 0) {
        for (cgltf_size i = 0; i < data->scenes[0].nodes_count; ++i) {
            ProcessNode(data->scenes[0].nodes[i], rootEntity, scene);
        }
    }

    // 解析 Skins
    for (cgltf_size s = 0; s < data->skins_count; ++s) {
        cgltf_skin* skin = &data->skins[s];
        auto skeleton = std::make_shared<Skeleton>();
        // 获取骨骼节点名称并添加骨骼
        for (cgltf_size j = 0; j < skin->joints_count; ++j) {
            cgltf_node* joint = skin->joints[j];
            std::string jointName = joint->name ? joint->name : "Joint";
            int parentIndex = -1;
            if (joint->parent) {
                // 查找父节点在骨骼数组中的索引
                for (cgltf_size k = 0; k < skin->joints_count; ++k) {
                    if (skin->joints[k] == joint->parent) {
                        parentIndex = (int)k;
                        break;
                    }
                }
            }
            glm::mat4 invBind = glm::mat4(1.0f);
            if (skin->inverse_bind_matrices) {
                // 读取逆绑定矩阵
                cgltf_accessor* invBindAccessor = skin->inverse_bind_matrices;
                cgltf_buffer_view* view = invBindAccessor->buffer_view;
                char* dataPtr = (char*)view->buffer->data + view->offset + invBindAccessor->offset + j * invBindAccessor->stride;
                invBind = ToGLMMat4((float*)dataPtr);
            }
            skeleton->AddBone(jointName, parentIndex, invBind);
        }
        // 将 skeleton 附加到对应的实体（需要找到皮肤对应的网格实体）
        // 简化：找到第一个使用该皮肤的 mesh 实体，并添加 Animator 组件
        for (auto& pair : s_nodeToEntity) {
            Entity* ent = pair.second;
            auto skinned = ent->GetComponent<SkinnedMeshRenderer>();
            if (skinned) {
                auto animator = ent->AddComponent<Animator>();
                animator->SetSkeleton(skeleton);
                skinned->SetAnimator(animator);
                break;
            }
        }
    }

// 解析 Animations
    for (cgltf_size a = 0; a < data->animations_count; ++a) {
        cgltf_animation* anim = &data->animations[a];
        auto clip = std::make_shared<AnimationClip>(anim->name ? anim->name : "Animation");
        float duration = 0.0f;

        for (cgltf_size c = 0; c < anim->channels_count; ++c) {
            cgltf_animation_channel* channel = &anim->channels[c];
            cgltf_animation_sampler* sampler = channel->sampler;
            std::string targetNodeName = channel->target_node->name ? channel->target_node->name : "Node";

            AnimationChannel animChannel;
            animChannel.nodeName = targetNodeName;

            cgltf_accessor* input = sampler->input;
            cgltf_accessor* output = sampler->output;
            if (input && output) {
                // 时间戳
                float* timeData = (float*)((char*)input->buffer_view->buffer->data + input->buffer_view->offset + input->offset);
                for (cgltf_size i = 0; i < input->count; ++i) {
                    animChannel.timeStamps.push_back(timeData[i]);
                    if (timeData[i] > duration) duration = timeData[i];
                }

                // 输出值（平移/旋转/缩放）
                if (channel->target_path == cgltf_animation_path_type_translation) {
                    float* outData = (float*)((char*)output->buffer_view->buffer->data + output->buffer_view->offset + output->offset);
                    for (cgltf_size i = 0; i < output->count; ++i) {
                        glm::vec3 t(outData[i*3], outData[i*3+1], outData[i*3+2]);
                        animChannel.translations.push_back(t);
                    }
                } else if (channel->target_path == cgltf_animation_path_type_rotation) {
                    float* outData = (float*)((char*)output->buffer_view->buffer->data + output->buffer_view->offset + output->offset);
                    for (cgltf_size i = 0; i < output->count; ++i) {
                        glm::quat q(outData[i*4+3], outData[i*4], outData[i*4+1], outData[i*4+2]);
                        animChannel.rotations.push_back(q);
                    }
                } else if (channel->target_path == cgltf_animation_path_type_scale) {
                    float* outData = (float*)((char*)output->buffer_view->buffer->data + output->buffer_view->offset + output->offset);
                    for (cgltf_size i = 0; i < output->count; ++i) {
                        glm::vec3 s(outData[i*3], outData[i*3+1], outData[i*3+2]);
                        animChannel.scales.push_back(s);
                    }
                }
                clip->AddChannel(animChannel);
            }
        }
        clip->SetDuration(duration);

        // 为根实体创建 Animator 组件，并设置节点映射和动画片段
        auto animator = rootEntity->AddComponent<Animator>();
        animator->SetNodeMap(s_nodeToEntity);
        animator->SetAnimationClip(clip);
        animator->Play(1.0f);
    }
    cgltf_free(data);
    return rootEntity;
}