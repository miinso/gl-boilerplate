/**
 * glfw3webgpu - GLFW <-> WebGPU surface creation adapter
 *
 * Based on https://github.com/eliemichel/glfw3webgpu
 * MIT License - Copyright (c) 2022-2024 Elie Michel and the wgpu-native authors
 */

#ifndef _glfw3_webgpu_h_
#define _glfw3_webgpu_h_

#include <webgpu/webgpu.h>
#include <GLFW/glfw3.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a WGPUSurface for the given GLFW window.
 * Returns NULL on failure. Caller must release with wgpuSurfaceRelease.
 */
WGPUSurface glfwCreateWindowWGPUSurface(WGPUInstance instance, GLFWwindow* window);

#ifdef __cplusplus
}
#endif

#endif /* _glfw3_webgpu_h_ */
