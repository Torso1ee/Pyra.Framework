#include "pv/PvBufferVma.h"
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <volk.h>
namespace Pyra {

class Material;

class ShaderVariant{
    //TODO
};
struct VertexAttribute {
  VkFormat format = VK_FORMAT_UNDEFINED;

  std::uint32_t stride = 0;

  std::uint32_t offset = 0;
};

class PvMesh {
public:
  PvMesh(const std::string &name = {});

  virtual ~PvMesh() = default;

  virtual std::type_index get_type();

  VkIndexType index_type{};

  std::uint32_t index_offset = 0;

  std::uint32_t vertices_count = 0;

  std::uint32_t vertex_indices = 0;

  std::unordered_map<std::string, std::shared_ptr<PvBufferVma>> vertex_buffers;

  std::shared_ptr<PvBufferVma> index_buffer;

  void set_attribute(const std::string &name, const VertexAttribute &attribute);

  bool get_attribute(const std::string &name, VertexAttribute &attribute) const;

  void set_material(const Material &material);

  const Material *get_material() const;

  const ShaderVariant &get_shader_variant() const;

  ShaderVariant &get_mut_shader_variant();

private:
  std::unordered_map<std::string, VertexAttribute> vertex_attributes;

  const Material *material{nullptr};

  ShaderVariant shader_variant;

  void compute_shader_variant();
};
} // namespace Pyra