/**
 * glfw3webgpu - GLFW <-> WebGPU surface creation adapter
 *
 * Based on https://github.com/eliemichel/glfw3webgpu
 * MIT License - Copyright (c) 2022-2025 Elie Michel and the wgpu-native authors
 */

#ifdef _WIN32
  #include <windows.h>
  #define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
  #define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(__linux__)
  #define GLFW_EXPOSE_NATIVE_X11
  #define GLFW_EXPOSE_NATIVE_WAYLAND
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <webgpu/webgpu.h>

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
    /* runtime detect: prefer wayland (works with wgpu GL+EGL backend), fall back to x11 */
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
        struct wl_display* wl_display = glfwGetWaylandDisplay();
        struct wl_surface* wl_surface = glfwGetWaylandWindow(window);
        WGPUSurfaceSourceWaylandSurface fromWayland = {
            .chain = { .next = NULL, .sType = WGPUSType_SurfaceSourceWaylandSurface },
            .display = wl_display,
            .surface = wl_surface,
        };
        WGPUSurfaceDescriptor desc = {
            .nextInChain = &fromWayland.chain,
            .label = { .data = NULL, .length = 0 },
        };
        return wgpuInstanceCreateSurface(instance, &desc);
    }
    if (glfwGetPlatform() == GLFW_PLATFORM_X11) {
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
    return NULL;
#else
    (void)instance;
    (void)window;
    return NULL;
#endif
}
