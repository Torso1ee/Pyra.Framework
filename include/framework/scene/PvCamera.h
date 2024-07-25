#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include "core/Event.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace glm;
namespace Pyra {

struct PvCamera {

  mat4 view;
  mat4 proj;

  vec3 center;
  vec3 lookAt;

  float angle;
  float aspect;
  float zNear;
  float zFar;

  Event<PvCamera *> updated;

  void update();
  void update(PvCamera camera);
  void pan(vec3);
  void rotate(float, float);
  void zoom(vec2, float);
};
} // namespace Pyra