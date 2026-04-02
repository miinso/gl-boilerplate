#include <webgpu/webgpu.h>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <iostream>
#include <cmath>
#include <cstring>

// wgsl shader source
const char* shader_source = R"(
struct VSOut {
    @builtin(position) position: vec4<f32>,
    @location(0) color: vec3<f32>,
};

struct Uniforms {
    transform: mat4x4<f32>,
};

@group(0) @binding(0) var<uniform> u: Uniforms;

@vertex
fn vs_main(@location(0) pos: vec3<f32>, @location(1) color: vec3<f32>) -> VSOut {
    var out: VSOut;
    out.position = u.transform * vec4<f32>(pos, 1.0);
    out.color = color;
    return out;
}

@fragment
fn fs_main(@location(0) color: vec3<f32>) -> @location(0) vec4<f32> {
    return vec4<f32>(color, 1.0);
}
)";

struct Matrix4 {
    float data[16];

    void set_rotation_z(float angle_radians) {
        float c = cos(angle_radians);
        float s = sin(angle_radians);
        data[0] = c;    data[4] = -s;   data[8]  = 0.0f; data[12] = 0.0f;
        data[1] = s;    data[5] = c;    data[9]  = 0.0f; data[13] = 0.0f;
        data[2] = 0.0f; data[6] = 0.0f; data[10] = 1.0f; data[14] = 0.0f;
        data[3] = 0.0f; data[7] = 0.0f; data[11] = 0.0f; data[15] = 1.0f;
    }
};

// adapter/device request helpers using callbacks
static WGPUAdapter request_adapter_sync(WGPUInstance instance, WGPUSurface surface) {
    struct UserData { WGPUAdapter adapter; bool done; };
    UserData ud = { nullptr, false };

    WGPURequestAdapterOptions opts = {};
    opts.compatibleSurface = surface;
    opts.featureLevel = WGPUFeatureLevel_Core;
    opts.powerPreference = WGPUPowerPreference_HighPerformance;

    WGPURequestAdapterCallbackInfo cb = {};
    cb.mode = WGPUCallbackMode_AllowSpontaneous;
    cb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                     WGPUStringView message, void* ud1, void*) {
        auto* data = static_cast<UserData*>(ud1);
        if (status == WGPURequestAdapterStatus_Success) {
            data->adapter = adapter;
        } else {
            std::cerr << "adapter request failed: "
                      << std::string(message.data, message.length) << std::endl;
        }
        data->done = true;
    };
    cb.userdata1 = &ud;

    wgpuInstanceRequestAdapter(instance, &opts, cb);
    while (!ud.done) {
        wgpuInstanceProcessEvents(instance);
    }
    return ud.adapter;
}

static WGPUDevice request_device_sync(WGPUInstance instance, WGPUAdapter adapter) {
    struct UserData { WGPUDevice device; bool done; };
    UserData ud = { nullptr, false };

    WGPUDeviceDescriptor desc = {};
    desc.label = { "device", WGPU_STRLEN };
    desc.defaultQueue.label = { "queue", WGPU_STRLEN };

    WGPURequestDeviceCallbackInfo cb = {};
    cb.mode = WGPUCallbackMode_AllowSpontaneous;
    cb.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                     WGPUStringView message, void* ud1, void*) {
        auto* data = static_cast<UserData*>(ud1);
        if (status == WGPURequestDeviceStatus_Success) {
            data->device = device;
        } else {
            std::cerr << "device request failed: "
                      << std::string(message.data, message.length) << std::endl;
        }
        data->done = true;
    };
    cb.userdata1 = &ud;

    wgpuAdapterRequestDevice(adapter, &desc, cb);
    while (!ud.done) {
        wgpuInstanceProcessEvents(instance);
    }
    return ud.device;
}

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // no OpenGL context -- we're using WebGPU
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window = glfwCreateWindow(800, 600, "WebGPU Triangle (wgpu-native)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // create wgpu instance
    WGPUInstanceDescriptor instance_desc = {};
    WGPUInstance instance = wgpuCreateInstance(&instance_desc);
    if (!instance) {
        std::cerr << "Failed to create WebGPU instance" << std::endl;
        return -1;
    }

    // create surface from glfw window
    WGPUSurface surface = glfwCreateWindowWGPUSurface(instance, window);
    if (!surface) {
        std::cerr << "Failed to create WebGPU surface" << std::endl;
        return -1;
    }

    // request adapter & device
    WGPUAdapter adapter = request_adapter_sync(instance, surface);
    if (!adapter) {
        std::cerr << "Failed to get WebGPU adapter" << std::endl;
        return -1;
    }

    WGPUDevice device = request_device_sync(instance, adapter);
    if (!device) {
        std::cerr << "Failed to get WebGPU device" << std::endl;
        return -1;
    }

    WGPUQueue queue = wgpuDeviceGetQueue(device);

    // print adapter info
    WGPUAdapterInfo info = {};
    wgpuAdapterGetInfo(adapter, &info);
    std::cout << "WebGPU Adapter: "
              << std::string(info.description.data, info.description.length) << std::endl;
    std::cout << "Backend: "
              << std::string(info.vendor.data, info.vendor.length) << std::endl;

    // query surface format
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(surface, adapter, &caps);
    WGPUTextureFormat surface_format = caps.formats[0];

    // configure surface
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    WGPUSurfaceConfiguration surface_config = {};
    surface_config.device = device;
    surface_config.format = surface_format;
    surface_config.usage = WGPUTextureUsage_RenderAttachment;
    surface_config.width = (uint32_t)fb_width;
    surface_config.height = (uint32_t)fb_height;
    surface_config.presentMode = WGPUPresentMode_Fifo;
    surface_config.alphaMode = caps.alphaModes[0];
    wgpuSurfaceConfigure(surface, &surface_config);

    // create shader module
    WGPUShaderSourceWGSL wgsl_source = {};
    wgsl_source.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgsl_source.code = { shader_source, WGPU_STRLEN };

    WGPUShaderModuleDescriptor shader_desc = {};
    shader_desc.nextInChain = &wgsl_source.chain;
    shader_desc.label = { "shader", WGPU_STRLEN };
    WGPUShaderModule shader_module = wgpuDeviceCreateShaderModule(device, &shader_desc);
    if (!shader_module) {
        std::cerr << "Failed to create shader module" << std::endl;
        return -1;
    }

    // vertex data: positions + colors (same layout as GL samples)
    float vertices[] = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // red
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // green
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // blue
    };

    // create vertex buffer
    WGPUBufferDescriptor buf_desc = {};
    buf_desc.label = { "vertices", WGPU_STRLEN };
    buf_desc.usage = WGPUBufferUsage_Vertex | WGPUBufferUsage_CopyDst;
    buf_desc.size = sizeof(vertices);
    WGPUBuffer vertex_buffer = wgpuDeviceCreateBuffer(device, &buf_desc);
    wgpuQueueWriteBuffer(queue, vertex_buffer, 0, vertices, sizeof(vertices));

    // create uniform buffer for transform matrix
    WGPUBufferDescriptor uniform_desc = {};
    uniform_desc.label = { "uniforms", WGPU_STRLEN };
    uniform_desc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    uniform_desc.size = sizeof(float) * 16;
    WGPUBuffer uniform_buffer = wgpuDeviceCreateBuffer(device, &uniform_desc);

    // create bind group layout + bind group for uniforms
    WGPUBindGroupLayoutEntry bgl_entry = {};
    bgl_entry.binding = 0;
    bgl_entry.visibility = WGPUShaderStage_Vertex;
    bgl_entry.buffer.type = WGPUBufferBindingType_Uniform;
    bgl_entry.buffer.minBindingSize = sizeof(float) * 16;

    WGPUBindGroupLayoutDescriptor bgl_desc = {};
    bgl_desc.label = { "bind_group_layout", WGPU_STRLEN };
    bgl_desc.entryCount = 1;
    bgl_desc.entries = &bgl_entry;
    WGPUBindGroupLayout bind_group_layout = wgpuDeviceCreateBindGroupLayout(device, &bgl_desc);

    WGPUBindGroupEntry bg_entry = {};
    bg_entry.binding = 0;
    bg_entry.buffer = uniform_buffer;
    bg_entry.offset = 0;
    bg_entry.size = sizeof(float) * 16;

    WGPUBindGroupDescriptor bg_desc = {};
    bg_desc.label = { "bind_group", WGPU_STRLEN };
    bg_desc.layout = bind_group_layout;
    bg_desc.entryCount = 1;
    bg_desc.entries = &bg_entry;
    WGPUBindGroup bind_group = wgpuDeviceCreateBindGroup(device, &bg_desc);

    // pipeline layout
    WGPUPipelineLayoutDescriptor pl_desc = {};
    pl_desc.label = { "pipeline_layout", WGPU_STRLEN };
    pl_desc.bindGroupLayoutCount = 1;
    pl_desc.bindGroupLayouts = &bind_group_layout;
    WGPUPipelineLayout pipeline_layout = wgpuDeviceCreatePipelineLayout(device, &pl_desc);

    // vertex attributes
    WGPUVertexAttribute attribs[2] = {};
    attribs[0].format = WGPUVertexFormat_Float32x3;
    attribs[0].offset = 0;
    attribs[0].shaderLocation = 0;
    attribs[1].format = WGPUVertexFormat_Float32x3;
    attribs[1].offset = 3 * sizeof(float);
    attribs[1].shaderLocation = 1;

    WGPUVertexBufferLayout vb_layout = {};
    vb_layout.arrayStride = 6 * sizeof(float);
    vb_layout.stepMode = WGPUVertexStepMode_Vertex;
    vb_layout.attributeCount = 2;
    vb_layout.attributes = attribs;

    // color target
    WGPUColorTargetState color_target = {};
    color_target.format = surface_format;
    color_target.writeMask = WGPUColorWriteMask_All;

    // fragment state
    WGPUFragmentState frag_state = {};
    frag_state.module = shader_module;
    frag_state.entryPoint = { "fs_main", WGPU_STRLEN };
    frag_state.targetCount = 1;
    frag_state.targets = &color_target;

    // render pipeline
    WGPURenderPipelineDescriptor pipeline_desc = {};
    pipeline_desc.label = { "pipeline", WGPU_STRLEN };
    pipeline_desc.layout = pipeline_layout;
    pipeline_desc.vertex.module = shader_module;
    pipeline_desc.vertex.entryPoint = { "vs_main", WGPU_STRLEN };
    pipeline_desc.vertex.bufferCount = 1;
    pipeline_desc.vertex.buffers = &vb_layout;
    pipeline_desc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipeline_desc.primitive.frontFace = WGPUFrontFace_CCW;
    pipeline_desc.primitive.cullMode = WGPUCullMode_None;
    pipeline_desc.multisample.count = 1;
    pipeline_desc.multisample.mask = 0xFFFFFFFF;
    pipeline_desc.fragment = &frag_state;

    WGPURenderPipeline pipeline = wgpuDeviceCreateRenderPipeline(device, &pipeline_desc);
    if (!pipeline) {
        std::cerr << "Failed to create render pipeline" << std::endl;
        return -1;
    }

    std::cout << "WebGPU pipeline created successfully" << std::endl;

    Matrix4 transform;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        // handle resize
        int cur_w, cur_h;
        glfwGetFramebufferSize(window, &cur_w, &cur_h);
        if (cur_w != (int)surface_config.width || cur_h != (int)surface_config.height) {
            if (cur_w > 0 && cur_h > 0) {
                surface_config.width = (uint32_t)cur_w;
                surface_config.height = (uint32_t)cur_h;
                wgpuSurfaceConfigure(surface, &surface_config);
            }
        }

        // update uniform
        float time = (float)glfwGetTime();
        transform.set_rotation_z(time);
        wgpuQueueWriteBuffer(queue, uniform_buffer, 0, transform.data, sizeof(transform.data));

        // get current texture
        WGPUSurfaceTexture surface_texture = {};
        wgpuSurfaceGetCurrentTexture(surface, &surface_texture);
        if (surface_texture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
            surface_texture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
            continue;
        }

        WGPUTextureView view = wgpuTextureCreateView(surface_texture.texture, nullptr);

        // render pass
        WGPURenderPassColorAttachment color_attachment = {};
        color_attachment.view = view;
        color_attachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
        color_attachment.loadOp = WGPULoadOp_Clear;
        color_attachment.storeOp = WGPUStoreOp_Store;
        color_attachment.clearValue = { 0.2, 0.2, 0.2, 1.0 };

        WGPURenderPassDescriptor rp_desc = {};
        rp_desc.colorAttachmentCount = 1;
        rp_desc.colorAttachments = &color_attachment;

        WGPUCommandEncoderDescriptor enc_desc = {};
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &enc_desc);
        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &rp_desc);

        wgpuRenderPassEncoderSetPipeline(pass, pipeline);
        wgpuRenderPassEncoderSetBindGroup(pass, 0, bind_group, 0, nullptr);
        wgpuRenderPassEncoderSetVertexBuffer(pass, 0, vertex_buffer, 0, sizeof(vertices));
        wgpuRenderPassEncoderDraw(pass, 3, 1, 0, 0);
        wgpuRenderPassEncoderEnd(pass);

        WGPUCommandBufferDescriptor cmd_desc = {};
        WGPUCommandBuffer cmd_buf = wgpuCommandEncoderFinish(encoder, &cmd_desc);
        wgpuQueueSubmit(queue, 1, &cmd_buf);
        wgpuSurfacePresent(surface);

        // release per-frame resources
        wgpuCommandBufferRelease(cmd_buf);
        wgpuCommandEncoderRelease(encoder);
        wgpuRenderPassEncoderRelease(pass);
        wgpuTextureViewRelease(view);
        wgpuTextureRelease(surface_texture.texture);
    }

    // cleanup
    wgpuRenderPipelineRelease(pipeline);
    wgpuPipelineLayoutRelease(pipeline_layout);
    wgpuBindGroupRelease(bind_group);
    wgpuBindGroupLayoutRelease(bind_group_layout);
    wgpuBufferRelease(uniform_buffer);
    wgpuBufferRelease(vertex_buffer);
    wgpuShaderModuleRelease(shader_module);
    wgpuSurfaceRelease(surface);
    wgpuDeviceRelease(device);
    wgpuAdapterRelease(adapter);
    wgpuInstanceRelease(instance);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
