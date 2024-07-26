#include "util/GltfLoader.h"
#include "VkBootstrap.h"
#include "core/geom.h"
#include "core/logging.h"
#include "pv/Pv.h"
#include "pv/PvBufferVma.h"
#include "pv/PvCommon.h"
#include "pv/PvQueue.h"
#include "util/helpers.h"
#include <glm/gtc/type_ptr.hpp>
#include <memory>
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

inline size_t get_attribute_stride(const tinygltf::Model *model,
                                   uint32_t accessorId) {
  assert(accessorId < model->accessors.size());
  auto &accessor = model->accessors[accessorId];
  assert(accessor.bufferView < model->bufferViews.size());
  auto &bufferView = model->bufferViews[accessor.bufferView];

  return accessor.ByteStride(bufferView);
};

inline VkFormat get_attribute_format(const tinygltf::Model *model,
                                     uint32_t accessorId) {
  assert(accessorId < model->accessors.size());
  auto &accessor = model->accessors[accessorId];

  VkFormat format;

  switch (accessor.componentType) {
  case TINYGLTF_COMPONENT_TYPE_BYTE: {
    static const std::map<int, VkFormat> mapped_format = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R8_SINT},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R8G8_SINT},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R8G8B8_SINT},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R8G8B8A8_SINT}};

    format = mapped_format.at(accessor.type);

    break;
  }
  case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
    static const std::map<int, VkFormat> mapped_format = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R8_UINT},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R8G8_UINT},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R8G8B8_UINT},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R8G8B8A8_UINT}};

    static const std::map<int, VkFormat> mapped_format_normalize = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R8_UNORM},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R8G8_UNORM},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R8G8B8_UNORM},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R8G8B8A8_UNORM}};

    if (accessor.normalized) {
      format = mapped_format_normalize.at(accessor.type);
    } else {
      format = mapped_format.at(accessor.type);
    }

    break;
  }
  case TINYGLTF_COMPONENT_TYPE_SHORT: {
    static const std::map<int, VkFormat> mapped_format = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R8_SINT},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R8G8_SINT},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R8G8B8_SINT},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R8G8B8A8_SINT}};

    format = mapped_format.at(accessor.type);

    break;
  }
  case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
    static const std::map<int, VkFormat> mapped_format = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R16_UINT},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R16G16_UINT},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R16G16B16_UINT},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R16G16B16A16_UINT}};

    static const std::map<int, VkFormat> mapped_format_normalize = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R16_UNORM},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R16G16_UNORM},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R16G16B16_UNORM},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R16G16B16A16_UNORM}};

    if (accessor.normalized) {
      format = mapped_format_normalize.at(accessor.type);
    } else {
      format = mapped_format.at(accessor.type);
    }

    break;
  }
  case TINYGLTF_COMPONENT_TYPE_INT: {
    static const std::map<int, VkFormat> mapped_format = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R32_SINT},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R32G32_SINT},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R32G32B32_SINT},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R32G32B32A32_SINT}};

    format = mapped_format.at(accessor.type);

    break;
  }
  case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
    static const std::map<int, VkFormat> mapped_format = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R32_UINT},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R32G32_UINT},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R32G32B32_UINT},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R32G32B32A32_UINT}};

    format = mapped_format.at(accessor.type);

    break;
  }
  case TINYGLTF_COMPONENT_TYPE_FLOAT: {
    static const std::map<int, VkFormat> mapped_format = {
        {TINYGLTF_TYPE_SCALAR, VK_FORMAT_R32_SFLOAT},
        {TINYGLTF_TYPE_VEC2, VK_FORMAT_R32G32_SFLOAT},
        {TINYGLTF_TYPE_VEC3, VK_FORMAT_R32G32B32_SFLOAT},
        {TINYGLTF_TYPE_VEC4, VK_FORMAT_R32G32B32A32_SFLOAT}};

    format = mapped_format.at(accessor.type);

    break;
  }
  default: {
    format = VK_FORMAT_UNDEFINED;
    break;
  }
  }

  return format;
};

inline size_t get_attribute_size(const tinygltf::Model *model,
                                 uint32_t accessorId) {
  assert(accessorId < model->accessors.size());
  return model->accessors[accessorId].count;
};

inline std::vector<uint8_t> get_attribute_data(const tinygltf::Model *model,
                                               uint32_t accessorId) {
  assert(accessorId < model->accessors.size());
  auto &accessor = model->accessors[accessorId];
  assert(accessor.bufferView < model->bufferViews.size());
  auto &bufferView = model->bufferViews[accessor.bufferView];
  assert(bufferView.buffer < model->buffers.size());
  auto &buffer = model->buffers[bufferView.buffer];

  size_t stride = accessor.ByteStride(bufferView);
  size_t startByte = accessor.byteOffset + bufferView.byteOffset;
  size_t endByte = startByte + accessor.count * stride;

  return {buffer.data.begin() + startByte, buffer.data.begin() + endByte};
};

inline std::vector<uint8_t>
convert_underlying_data_stride(const std::vector<uint8_t> &src_data,
                               uint32_t src_stride, uint32_t dst_stride) {
  auto elem_count = to_u32(src_data.size()) / src_stride;

  std::vector<uint8_t> result(elem_count * dst_stride);

  for (uint32_t idxSrc = 0, idxDst = 0;
       idxSrc < src_data.size() && idxDst < result.size();
       idxSrc += src_stride, idxDst += dst_stride) {
    std::copy(src_data.begin() + idxSrc, src_data.begin() + idxSrc + src_stride,
              result.begin() + idxDst);
  }

  return result;
}

std::unique_ptr<PvMesh> GltfLoader::loadModel(uint32_t index,
                                              bool storage_buffer) {
  auto submesh = std::make_unique<PvMesh>();

  std::vector<std::shared_ptr<PvBufferVma>> transient_buffers;

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

    auto stage_buffer = bootstrap->createStagingBuffer(aligned_vertex_data);

    CreateInfo<PvBufferVma> bInfo{
        .size = aligned_vertex_data.size() * sizeof(AlignedVertex),
        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .allocInfo = {.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
                               VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
                      .usage = VMA_MEMORY_USAGE_GPU_ONLY}

    };

    auto buffer = bootstrap->make<PvBufferVma>(bInfo);

    commandBuffer.copyBuffer(
        stage_buffer->handle->buffer, buffer->handle->buffer,
        aligned_vertex_data.size() * sizeof(AlignedVertex));

    auto pair = std::make_pair("vertex_buffer", std::move(buffer));
    submesh->vertex_buffers.insert(std::move(pair));

    transient_buffers.push_back(stage_buffer);
  } else {
    for (size_t v = 0; v < vertex_count; v++) {
      Vertex vert{};
      vert.pos = glm::vec4(glm::make_vec3(&pos[v * 3]), 1.0f);
      vert.normal = glm::normalize(glm::vec3(
          normals ? glm::make_vec3(&normals[v * 3]) : glm::vec3(0.0f)));
      vert.uv = uvs ? glm::make_vec2(&uvs[v * 2]) : glm::vec3(0.0f);

      vert.joint0 = has_skin ? glm::vec4(glm::make_vec4(&joints[v * 4]))
                             : glm::vec4(0.0f);
      vert.weight0 =
          has_skin ? glm::make_vec4(&weights[v * 4]) : glm::vec4(0.0f);
      vertex_data.push_back(vert);
    }

    auto stage_buffer = bootstrap->createStagingBuffer(aligned_vertex_data);

    CreateInfo<PvBufferVma> bInfo{
        .size = aligned_vertex_data.size() * sizeof(AlignedVertex),
        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .allocInfo = {.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
                               VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
                      .usage = VMA_MEMORY_USAGE_GPU_ONLY}

    };

    auto buffer = bootstrap->make<PvBufferVma>(bInfo);

    commandBuffer.copyBuffer(
        stage_buffer->handle->buffer, buffer->handle->buffer,
        aligned_vertex_data.size() * sizeof(AlignedVertex));

    auto pair = std::make_pair("vertex_buffer", std::move(buffer));
    submesh->vertex_buffers.insert(std::move(pair));
    transient_buffers.push_back(stage_buffer);
  }

  if (gltf_primitive.indices >= 0) {
    submesh->vertex_indices =
        to_u32(get_attribute_size(&model, gltf_primitive.indices));

    auto format = get_attribute_format(&model, gltf_primitive.indices);
    auto index_data = get_attribute_data(&model, gltf_primitive.indices);

    switch (format) {
    case VK_FORMAT_R32_UINT: {
      // Correct format
      break;
    }
    case VK_FORMAT_R16_UINT: {
      index_data = convert_underlying_data_stride(index_data, 2, 4);
      break;
    }
    case VK_FORMAT_R8_UINT: {
      index_data = convert_underlying_data_stride(index_data, 1, 4);
      break;
    }
    default: {
      break;
    }
    }

    // Always do uint32
    submesh->index_type = VK_INDEX_TYPE_UINT32;

    if (storage_buffer) {
      // prepare meshlets
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
    } else {
      //     core::Buffer stage_buffer =
      //         vkb::core::Buffer::create_staging_buffer(device, index_data);

      //     submesh->index_buffer = std::make_unique<core::Buffer>(
      //         device, index_data.size(),
      //         VK_BUFFER_USAGE_TRANSFER_DST_BIT |
      //         VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

      //     command_buffer.copy_buffer(stage_buffer, *submesh->index_buffer,
      //                                index_data.size());

      //     transient_buffers.push_back(std::move(stage_buffer));
    }
  }
  commandBuffer.endCommandBuffer();

  auto fence =
      bootstrap->make<PvFence>({.flags = VK_FENCE_CREATE_SIGNALED_BIT});

  SubmitInfo sInfo{
      .fence = fence->handle,
      .submitInfos = {
        {}
      }
  };
  queue->submit(sInfo);

  // device.get_fence_pool().wait();
  // device.get_fence_pool().reset();
  // device.get_command_pool().reset_pool();

  return std::move(submesh);
}

} // namespace Pyra