#include "pv/PvBufferVma.h"
#include "core/Core.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <cstdint>
#include <volk.h>

namespace Pyra {

void PvBufferVmaCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .flags = flags,
          .size = size,
          .usage = usage,
          .sharingMode = sharingMode,
          .queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size(),
          .pQueueFamilyIndices = NULLPTR_IF_EMPTY(queueFamilyIndices)};
}

PvBufferVma::~PvBufferVma() { clear(); }

bool PvBufferVma::init(PvBufferVmaCreateInfo &info) {
  table = info.table;
  handle = new VkBufferVma{};
  VmaAllocationInfo aInfo{};
  if (vmaCreateBuffer(info.table->allocator, &info.info, &info.allocInfo,
                      &handle->buffer, &handle->memory, &aInfo) != VK_SUCCESS) {
    ERROR("Failed to create vmaBuffer!");
    return false;
  }

  postCreate(aInfo);

  if (!setDctor)
    setDeconstructor(vmaDestroyBuffer);

  manage(handle,
         std::make_tuple(info.table->allocator, handle->buffer, handle->memory),
         info.operation, {info.table->device.device});
  return true;
}

void PvBufferVma::flush(VkDeviceSize offset, VkDeviceSize size) {
  if (!coherent) {
    vmaFlushAllocation(table->allocator, handle->memory, offset, size);
  }
}

size_t PvBufferVma::update(const uint8_t *data, size_t size, size_t offset) {
  if (mappedData != nullptr) {
    std::copy(data, data + size, mappedData + offset);
    flush();
  } else {
    map();
    std::copy(data, data + size, mappedData + offset);
    flush();
    unmap();
  }
  return size;
}

uint8_t *PvBufferVma::map() {
  if (!persistent && !mapped()) {
    VK_CHECK(vmaMapMemory(table->allocator, handle->memory,
                          reinterpret_cast<void **>(&mappedData)));
    assert(mappedData);
  }
  return mappedData;
}

void PvBufferVma::unmap() {
  if (!persistent && mapped()) {
    vmaUnmapMemory(table->allocator, handle->memory);
    mappedData = nullptr;
  }
}

bool PvBufferVma::mapped() const { return mappedData != nullptr; }

void PvBufferVma::clear() {
  if (handle->buffer != VK_NULL_HANDLE && handle->memory != VK_NULL_HANDLE) {
    unmap();
    clear();
  }
}

void PvBufferVma::postCreate(VmaAllocationInfo const &allocation_info) {
  VkMemoryPropertyFlags memory_properties;
  vmaGetAllocationMemoryProperties(table->allocator, handle->memory,
                                   &memory_properties);
  coherent = (memory_properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) ==
             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  mappedData = static_cast<uint8_t *>(allocation_info.pMappedData);
  persistent = mappedData == nullptr;
}

} // namespace Pyra