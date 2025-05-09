cmake_minimum_required(VERSION 3.27.0)
project(framework VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC

app/ApplicationBase.cpp
app/VulkanApplication.cpp

core/logging.cpp
core/GlslCompiler.cpp
core/VmaUsage.cpp
core/TinyObjLoaderUsage.cpp
core/TinyGltfUsage.cpp

pv/PvBootstrap.cpp
pv/PvCommon.cpp
pv/PvResource.cpp
pv/PvSwapchain.cpp
pv/PvInstance.cpp
pv/PvDebugUtilsMessenger.cpp
pv/PvPhysicalDevice.cpp
pv/PvDevice.cpp
pv/PvSurface.cpp
pv/PvImage.cpp
pv/PvImageView.cpp
pv/PvImageVma.cpp
pv/PvFramebuffer.cpp
pv/PvRenderPass.cpp
pv/PvPipeline.cpp
pv/PvPipelineLayout.cpp
pv/PvPipelineCache.cpp
pv/PvBuffer.cpp
pv/PvBufferVma.cpp
pv/PvShaderModule.cpp
pv/PvCommandPool.cpp
pv/PvCommandBuffers.cpp
pv/PvFence.cpp
pv/PvSemaphore.cpp
pv/PvQueue.cpp

util/GltfLoader.cpp

window/GlfwWindow.cpp
window/WindowBase.cpp
scene/PvCamera.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
${INCLUDE_DIR}/framework
$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_link_libraries(${PROJECT_NAME} PUBLIC glm spdlog glfw shaderc SPIRV-Tools-opt SPIRV vk-bootstrap::vk-bootstrap volk VulkanMemoryAllocator tinyobjloader tinygltf)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION}) 
include(CPack)