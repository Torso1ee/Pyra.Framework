#include "scene/PvCamera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

namespace Pyra {
void PvCamera::update() {
  view = glm::lookAt(center, lookAt, {0.0f, 0.0f, 1.0f});
  proj = glm::perspective(glm::radians(angle), aspect, zNear, zFar);
  updated.invoke(this);
}

void PvCamera::update(PvCamera cam) {
  center = cam.center;
  lookAt = cam.lookAt;
  angle = cam.angle;
  aspect = cam.aspect;
  zNear = cam.zNear;
  zFar = cam.zFar;
  update();
}

void PvCamera::pan(vec3 mov) {
  mov.x *= -1;
  auto pInverse = glm::inverse(proj);
  auto trans = glm::inverse(view);
  auto lookV = proj * view * vec4(lookAt, 1);
  lookV /= lookV.w;
  lookV.x += mov.x;
  lookV.y += mov.y;
  lookV = trans * pInverse * lookV;
  lookV /= lookV.w;
  auto v = vec3(lookV) - lookAt;
  center += v;
  lookAt += v;
  update();
}

void PvCamera::rotate(float xPos, float yPos) {
  vec3 diff = lookAt - center;
  vec3 dir = glm::normalize(diff);
  float x, y, z;
  auto v1 = glm::normalize(vec3(dir.x, dir.y, 0));
  v1 = glm::normalize(v1);
  x = glm::degrees(glm::angle(vec3(1, 0, 0), v1));
  if (v1.y < 0)
    x *= -1;
  y = glm::degrees(glm::angle(v1, dir));
  if (dir.z > 0)
    y *= -1;
  z = 0;
  x -= xPos;
  y += yPos;
  if (y > 89)
    y = 89;
  else if (y < -89)
    y = -89;
  auto rotationMatrix = glm::eulerAngleZYX(glm::radians(x), glm::radians(y), z);
  center =
      lookAt - (vec3(rotationMatrix * vec4(1, 0, 0, 1)) * glm::length(diff));
  update();
}

void PvCamera::zoom(vec2 pos, float factor) {
  pos.y *= -1;
  vec3 diff = lookAt - center;
  vec3 dir = glm::normalize(diff);
  auto pInverse = glm::inverse(proj);
  auto trans = glm::inverse(view);
  auto lookV = proj * view * vec4(lookAt, 1);
  lookV /= lookV.w;
  auto trl = glm::translate(glm::mat4(1.0f), -vec3(pos, lookV.z));
  auto scale = glm::scale(glm::mat4(1.0f), vec3(factor));
  auto trl1 = glm::translate(glm::mat4(1.0f), vec3(pos, lookV.z));
  lookV = trl1 * scale * trl * lookV;
  lookV /= lookV.w;
  lookV = trans * pInverse * lookV;
  lookV /= lookV.w;
  lookAt = vec3(lookV);
  center = lookAt - dir * glm::length(diff) * factor;
  zNear *= factor;
  zFar *= factor;
  update();
}

} // namespace Pyra