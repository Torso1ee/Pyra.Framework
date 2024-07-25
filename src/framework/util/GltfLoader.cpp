#include "util/GltfLoader.h"
#include "VkBootstrap.h"
#include "core/geom.h"
#include "core/logging.h"
#include "pv/Pv.h"
#include "pv/PvBufferVma.h"
#include "pv/PvCommon.h"
#include <glm/gtc/type_ptr.hpp>
#include <tiny_gltf.h>

namespace Pyra {

std::shared_ptr<PvMesh> GltfLoader::loadModelFromFile(const char *file) {

  std::string err;
  std::string warn;

  tinygltf::TinyGLTF gltf_loader;

  bool importResult = gltf_loader.LoadASCIIFromFile(&model, &err, &warn, file);

  if (!importResult) {
    ERROR("Failed to load gltf file {}.", file);

    return nullptr;
  }

  if (!err.empty()) {
    ERROR("Error loading gltf model: {}.", err.c_str());

    return nullptr;
  }

  if (!warn.empty()) {
    INFO("{}", warn.c_str());
  }

  return nullptr;
}

std::unique_ptr<PvMesh> GltfLoader::loadModel(uint32_t index,
                                              bool storage_buffer) {
  auto submesh = std::make_unique<PvMesh>();

  std::vector<PvBufferVma> transient_buffers;

  auto queue = bootstrap->getQueue(vkb::QueueType::graphics);

  auto commandBuffer = commandPool->requestCommandBuffer();

  commandBuffer.beginCommandBuffer(
      {.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT});

  assert(index < model.meshes.size());

  auto &gltf_mesh = model.meshes[index];

  assert(!gltf_mesh.primitives.empty());
  auto &gltf_primitive = gltf_mesh.primitives[0];

  std::vector<Vertex> vertex_data;
  std::vector<AlignedVertex> aligned_vertex_data;

  const float *pos = nullptr;
  const float *normals = nullptr;
  const float *uvs = nullptr;
  const uint16_t *joints = nullptr;
  const float *weights = nullptr;

  // Position attribute is required
  auto &accessor =
      model.accessors[gltf_primitive.attributes.find("POSITION")->second];
  size_t vertex_count = accessor.count;
  auto &buffer_view = model.bufferViews[accessor.bufferView];
  pos = reinterpret_cast<const float *>(
      &(model.buffers[buffer_view.buffer]
            .data[accessor.byteOffset + buffer_view.byteOffset]));

  if (gltf_primitive.attributes.find("NORMAL") !=
      gltf_primitive.attributes.end()) {
    accessor =
        model.accessors[gltf_primitive.attributes.find("NORMAL")->second];
    buffer_view = model.bufferViews[accessor.bufferView];
    normals = reinterpret_cast<const float *>(
        &(model.buffers[buffer_view.buffer]
              .data[accessor.byteOffset + buffer_view.byteOffset]));
  }

  if (gltf_primitive.attributes.find("TEXCOORD_0") !=
      gltf_primitive.attributes.end()) {
    accessor =
        model.accessors[gltf_primitive.attributes.find("TEXCOORD_0")->second];
    buffer_view = model.bufferViews[accessor.bufferView];
    uvs = reinterpret_cast<const float *>(
        &(model.buffers[buffer_view.buffer]
              .data[accessor.byteOffset + buffer_view.byteOffset]));
  }

  // Skinning
  // Joints
  if (gltf_primitive.attributes.find("JOINTS_0") !=
      gltf_primitive.attributes.end()) {
    accessor =
        model.accessors[gltf_primitive.attributes.find("JOINTS_0")->second];
    buffer_view = model.bufferViews[accessor.bufferView];
    joints = reinterpret_cast<const uint16_t *>(
        &(model.buffers[buffer_view.buffer]
              .data[accessor.byteOffset + buffer_view.byteOffset]));
  }

  if (gltf_primitive.attributes.find("WEIGHTS_0") !=
      gltf_primitive.attributes.end()) {
    accessor =
        model.accessors[gltf_primitive.attributes.find("WEIGHTS_0")->second];
    buffer_view = model.bufferViews[accessor.bufferView];
    weights = reinterpret_cast<const float *>(
        &(model.buffers[buffer_view.buffer]
              .data[accessor.byteOffset + buffer_view.byteOffset]));
  }

  bool has_skin = (joints && weights);

  if (storage_buffer) {
    for (size_t v = 0; v < vertex_count; v++) {
      AlignedVertex vert{};
      vert.pos = glm::vec4(glm::make_vec3(&pos[v * 3]), 1.0f);
      vert.normal =
          normals
              ? glm::vec4(glm::normalize(glm::make_vec3(&normals[v * 3])), 0.0f)
              : glm::vec4(0.0f);
      aligned_vertex_data.push_back(vert);
    }

    CreateInfo<PvBufferVma> info{
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .allocInfo = {
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
                     VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT}};
    auto stage_buffer = bootstrap->make<PvBufferVma>(info);

    //   core::Buffer stage_buffer =
    //       vkb::core::Buffer::create_staging_buffer(device,
    //       aligned_vertex_data);

    //   core::Buffer buffer{
    //       device, aligned_vertex_data.size() * sizeof(AlignedVertex),
    //       VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    //       VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY};

    //   command_buffer.copy_buffer(stage_buffer, buffer,
    //                              aligned_vertex_data.size() *
    //                                  sizeof(AlignedVertex));

    //   auto pair = std::make_pair("vertex_buffer", std::move(buffer));
    //   submesh->vertex_buffers.insert(std::move(pair));

    //   transient_buffers.push_back(std::move(stage_buffer));
    // } else {
    //   for (size_t v = 0; v < vertex_count; v++) {
    //     Vertex vert{};
    //     vert.pos = glm::vec4(glm::make_vec3(&pos[v * 3]), 1.0f);
    //     vert.normal = glm::normalize(glm::vec3(
    //         normals ? glm::make_vec3(&normals[v * 3]) : glm::vec3(0.0f)));
    //     vert.uv = uvs ? glm::make_vec2(&uvs[v * 2]) : glm::vec3(0.0f);

    //     vert.joint0 = has_skin ? glm::vec4(glm::make_vec4(&joints[v * 4]))
    //                            : glm::vec4(0.0f);
    //     vert.weight0 =
    //         has_skin ? glm::make_vec4(&weights[v * 4]) : glm::vec4(0.0f);
    //     vertex_data.push_back(vert);
    //   }

    //   core::Buffer stage_buffer =
    //       vkb::core::Buffer::create_staging_buffer(device, vertex_data);

    //   core::Buffer buffer{device, vertex_data.size() * sizeof(Vertex),
    //                       VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    //                           VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    //                       VMA_MEMORY_USAGE_GPU_ONLY};

    //   command_buffer.copy_buffer(stage_buffer, buffer,
    //                              vertex_data.size() * sizeof(Vertex));

    //   auto pair = std::make_pair("vertex_buffer", std::move(buffer));
    //   submesh->vertex_buffers.insert(std::move(pair));

    //   transient_buffers.push_back(std::move(stage_buffer));
    // }

    // if (gltf_primitive.indices >= 0) {
    //   submesh->vertex_indices =
    //       to_u32(get_attribute_size(&model, gltf_primitive.indices));

    //   auto format = get_attribute_format(&model, gltf_primitive.indices);
    //   auto index_data = get_attribute_data(&model, gltf_primitive.indices);

    //   switch (format) {
    //   case VK_FORMAT_R32_UINT: {
    //     // Correct format
    //     break;
    //   }
    //   case VK_FORMAT_R16_UINT: {
    //     index_data = convert_underlying_data_stride(index_data, 2, 4);
    //     break;
    //   }
    //   case VK_FORMAT_R8_UINT: {
    //     index_data = convert_underlying_data_stride(index_data, 1, 4);
    //     break;
    //   }
    //   default: {
    //     break;
    //   }
    //   }

    //   // Always do uint32
    //   submesh->index_type = VK_INDEX_TYPE_UINT32;

    //   if (storage_buffer) {
    //     // prepare meshlets
    //     std::vector<Meshlet> meshlets;
    //     prepare_meshlets(meshlets, submesh, index_data);

    //     // vertex_indices and index_buffer are used for meshlets now
    //     submesh->vertex_indices = static_cast<uint32_t>(meshlets.size());

    //     core::Buffer stage_buffer =
    //         vkb::core::Buffer::create_staging_buffer(device, meshlets);

    //     submesh->index_buffer = std::make_unique<core::Buffer>(
    //         device, meshlets.size() * sizeof(Meshlet),
    //         VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    //         VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

    //     command_buffer.copy_buffer(stage_buffer, *submesh->index_buffer,
    //                                meshlets.size() * sizeof(Meshlet));

    //     transient_buffers.push_back(std::move(stage_buffer));
    //   } else {
    //     core::Buffer stage_buffer =
    //         vkb::core::Buffer::create_staging_buffer(device, index_data);

    //     submesh->index_buffer = std::make_unique<core::Buffer>(
    //         device, index_data.size(),
    //         VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    //         VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

    //     command_buffer.copy_buffer(stage_buffer, *submesh->index_buffer,
    //                                index_data.size());

    //     transient_buffers.push_back(std::move(stage_buffer));
    //   }
  }

  // command_buffer.end();

  // queue.submit(command_buffer, device.request_fence());

  // device.get_fence_pool().wait();
  // device.get_fence_pool().reset();
  // device.get_command_pool().reset_pool();

  return std::move(submesh);
}

} // namespace Pyra