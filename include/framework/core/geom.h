#include <glm/glm.hpp>

namespace Pyra {

struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 uv;
  glm::vec4 joint0;
  glm::vec4 weight0;
};

struct AlignedVertex
{
	glm::vec4 pos;
	glm::vec4 normal;
};


} // namespace Pyra