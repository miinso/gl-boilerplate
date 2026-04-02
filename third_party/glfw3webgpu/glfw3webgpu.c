/**
 * glfw3webgpu - GLFW <-> WebGPU surface creation adapter
 *
 * Based on https://github.com/eliemichel/glfw3webgpu
 * MIT License - Copyright (c) 2022-2025 Elie Michel and the wgpu-native authors
 */

#include "glfw3webgpu.h"

#ifdef _WIN32
  #include <windows.h>
  #define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
  #define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(__linux__)
  #define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <webgpu/webgpu.h>

#ifdef __APPLE__
  #include <Foundation/Foundation.h>
  #include <QuartzCore/CAMetalLayer.h>
#endif

WGPUSurface glfwCreateWindowWGPUSurface(WGPUInstance instance, GLFWwindow* window) {
#ifdef _WIN32
    {
        HWND hwnd = glfwGetWin32Window(window);
        HINSTANCE hinstance = GetModuleHandle(NULL);
        WGPUSurfaceSourceWindowsHWND fromHwnd = {
            .chain = { .next = NULL, .sType = WGPUSType_SurfaceSourceWindowsHWND },
            .hinstance = hinstance,
            .hwnd = hwnd,
        };
        WGPUSurfaceDescriptor desc = {
            .nextInChain = &fromHwnd.chain,
            .label = { .data = NULL, .length = 0 },
        };
        return wgpuInstanceCreateSurface(instance, &desc);
    }
#elif defined(__APPLE__)
    {
        id ns_window = glfwGetCocoaWindow(window);
        NSView* content_view = [ns_window contentView];
        [content_view setWantsLayer:YES];
        CAMetalLayer* metal_layer = [CAMetalLayer layer];
        [content_view setLayer:metal_layer];
        WGPUSurfaceSourceMetalLayer fromMetal = {
            .chain = { .next = NULL, .sType = WGPUSType_SurfaceSourceMetalLayer },
            .layer = metal_layer,
        };
        WGPUSurfaceDescriptor desc = {
            .nextInChain = &fromMetal.chain,
            .label = { .data = NULL, .length = 0 },
        };
        return wgpuInstanceCreateSurface(instance, &desc);
    }
#elif defined(__linux__)
    {
        Display* x11_display = glfwGetX11Display();
        Window x11_window = glfwGetX11Window(window);
        WGPUSurfaceSourceXlibWindow fromXlib = {
            .chain = { .next = NULL, .sType = WGPUSType_SurfaceSourceXlibWindow },
            .display = x11_display,
            .window = (uint64_t)x11_window,
        };
        WGPUSurfaceDescriptor desc = {
            .nextInChain = &fromXlib.chain,
            .label = { .data = NULL, .length = 0 },
        };
        return wgpuInstanceCreateSurface(instance, &desc);
    }
#else
    (void)instance;
    (void)window;
    return NULL;
#endif
}
