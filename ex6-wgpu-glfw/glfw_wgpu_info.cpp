#include <webgpu/webgpu.h>
#include <webgpu/wgpu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// adapter/device request helpers
static WGPUAdapter request_adapter_sync(WGPUInstance instance) {
    struct UserData { WGPUAdapter adapter; bool done; };
    UserData ud = { nullptr, false };

    WGPURequestAdapterOptions opts = {};
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

static std::string sv(WGPUStringView s) {
    if (!s.data) return "(null)";
    return std::string(s.data, s.length);
}

static const char* backend_name(WGPUBackendType type) {
    switch (type) {
        case WGPUBackendType_Undefined: return "Undefined";
        case WGPUBackendType_Null:      return "Null";
        case WGPUBackendType_WebGPU:    return "WebGPU";
        case WGPUBackendType_D3D11:     return "D3D11";
        case WGPUBackendType_D3D12:     return "D3D12";
        case WGPUBackendType_Metal:     return "Metal";
        case WGPUBackendType_Vulkan:    return "Vulkan";
        case WGPUBackendType_OpenGL:    return "OpenGL";
        case WGPUBackendType_OpenGLES:  return "OpenGL ES";
        default:                        return "Unknown";
    }
}

static const char* adapter_type_name(WGPUAdapterType type) {
    switch (type) {
        case WGPUAdapterType_DiscreteGPU:   return "Discrete GPU";
        case WGPUAdapterType_IntegratedGPU: return "Integrated GPU";
        case WGPUAdapterType_CPU:           return "CPU (software)";
        case WGPUAdapterType_Unknown:       return "Unknown";
        default:                            return "Unknown";
    }
}

static const char* feature_name(WGPUFeatureName f) {
    switch (f) {
        case WGPUFeatureName_DepthClipControl:         return "DepthClipControl";
        case WGPUFeatureName_Depth32FloatStencil8:     return "Depth32FloatStencil8";
        case WGPUFeatureName_TimestampQuery:           return "TimestampQuery";
        case WGPUFeatureName_TextureCompressionBC:     return "TextureCompressionBC";
        case WGPUFeatureName_TextureCompressionETC2:   return "TextureCompressionETC2";
        case WGPUFeatureName_TextureCompressionASTC:   return "TextureCompressionASTC";
        case WGPUFeatureName_IndirectFirstInstance:    return "IndirectFirstInstance";
        case WGPUFeatureName_ShaderF16:                return "ShaderF16";
        case WGPUFeatureName_RG11B10UfloatRenderable:  return "RG11B10UfloatRenderable";
        case WGPUFeatureName_BGRA8UnormStorage:        return "BGRA8UnormStorage";
        case WGPUFeatureName_Float32Filterable:        return "Float32Filterable";
        case WGPUFeatureName_Float32Blendable:         return "Float32Blendable";
        case WGPUFeatureName_DualSourceBlending:       return "DualSourceBlending";
        default:                                       return nullptr;
    }
}

void print_adapter_info(WGPUAdapter adapter) {
    WGPUAdapterInfo info = {};
    wgpuAdapterGetInfo(adapter, &info);

    std::cout << "\nAdapter Information:" << std::endl;
    std::cout << "  Vendor:       " << sv(info.vendor) << std::endl;
    std::cout << "  Architecture: " << sv(info.architecture) << std::endl;
    std::cout << "  Device:       " << sv(info.device) << std::endl;
    std::cout << "  Description:  " << sv(info.description) << std::endl;
    std::cout << "  Backend:      " << backend_name(info.backendType) << std::endl;
    std::cout << "  Adapter Type: " << adapter_type_name(info.adapterType) << std::endl;
    std::cout << "  Vendor ID:    0x" << std::hex << info.vendorID << std::dec << std::endl;
    std::cout << "  Device ID:    0x" << std::hex << info.deviceID << std::dec << std::endl;

    wgpuAdapterInfoFreeMembers(info);
}

void print_adapter_features(WGPUAdapter adapter) {
    WGPUSupportedFeatures features = {};
    wgpuAdapterGetFeatures(adapter, &features);

    std::cout << "\nSupported Features (" << features.featureCount << "):" << std::endl;
    for (size_t i = 0; i < features.featureCount; i++) {
        const char* name = feature_name(features.features[i]);
        if (name) {
            std::cout << "  - " << name << std::endl;
        } else {
            std::cout << "  - (vendor/ext 0x" << std::hex << features.features[i]
                      << std::dec << ")" << std::endl;
        }
    }

    wgpuSupportedFeaturesFreeMembers(features);
}

void print_limits(WGPUAdapter adapter) {
    WGPULimits limits = {};
    WGPUAdapterInfo info = {};

    WGPUStatus status = wgpuAdapterGetLimits(adapter, &limits);
    if (status != WGPUStatus_Success) {
        std::cerr << "  failed to get limits" << std::endl;
        return;
    }

    std::cout << "\nLimits:" << std::endl;
    std::cout << "  maxTextureDimension1D:          " << limits.maxTextureDimension1D << std::endl;
    std::cout << "  maxTextureDimension2D:          " << limits.maxTextureDimension2D << std::endl;
    std::cout << "  maxTextureDimension3D:          " << limits.maxTextureDimension3D << std::endl;
    std::cout << "  maxTextureArrayLayers:          " << limits.maxTextureArrayLayers << std::endl;
    std::cout << "  maxBindGroups:                  " << limits.maxBindGroups << std::endl;
    std::cout << "  maxBindingsPerBindGroup:        " << limits.maxBindingsPerBindGroup << std::endl;
    std::cout << "  maxBufferSize:                  " << limits.maxBufferSize << std::endl;
    std::cout << "  maxVertexBuffers:               " << limits.maxVertexBuffers << std::endl;
    std::cout << "  maxVertexAttributes:            " << limits.maxVertexAttributes << std::endl;
    std::cout << "  maxVertexBufferArrayStride:     " << limits.maxVertexBufferArrayStride << std::endl;
    std::cout << "  maxSampledTexturesPerShaderStage: " << limits.maxSampledTexturesPerShaderStage << std::endl;
    std::cout << "  maxSamplersPerShaderStage:      " << limits.maxSamplersPerShaderStage << std::endl;
    std::cout << "  maxStorageBuffersPerShaderStage: " << limits.maxStorageBuffersPerShaderStage << std::endl;
    std::cout << "  maxStorageTexturesPerShaderStage: " << limits.maxStorageTexturesPerShaderStage << std::endl;
    std::cout << "  maxUniformBuffersPerShaderStage: " << limits.maxUniformBuffersPerShaderStage << std::endl;
    std::cout << "  maxUniformBufferBindingSize:    " << limits.maxUniformBufferBindingSize << std::endl;
    std::cout << "  maxStorageBufferBindingSize:    " << limits.maxStorageBufferBindingSize << std::endl;
    std::cout << "  maxComputeWorkgroupSizeX:       " << limits.maxComputeWorkgroupSizeX << std::endl;
    std::cout << "  maxComputeWorkgroupSizeY:       " << limits.maxComputeWorkgroupSizeY << std::endl;
    std::cout << "  maxComputeWorkgroupSizeZ:       " << limits.maxComputeWorkgroupSizeZ << std::endl;
    std::cout << "  maxComputeWorkgroupsPerDimension: " << limits.maxComputeWorkgroupsPerDimension << std::endl;
    std::cout << "  maxColorAttachments:            " << limits.maxColorAttachments << std::endl;
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

    // print glfw info
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    std::cout << "GLFW Version: " << major << "." << minor << "." << rev << std::endl;
    std::cout << "Vulkan supported: " << (glfwVulkanSupported() ? "yes" : "no") << std::endl;

    // create wgpu instance (disable validation to skip indirect validation
    // compute pipeline that fails on some GL drivers e.g. WSL2 Mesa D3D12)
    WGPUInstanceExtras extras = {};
    extras.chain.sType = (WGPUSType)WGPUSType_InstanceExtras;
    extras.backends = WGPUInstanceBackend_All;
    extras.flags = WGPUInstanceFlag_Debug;  // no Validation -- skip indirect validation compute pipeline

    WGPUInstanceDescriptor instance_desc = {};
    instance_desc.nextInChain = &extras.chain;
    WGPUInstance instance = wgpuCreateInstance(&instance_desc);
    if (!instance) {
        std::cerr << "Failed to create WebGPU instance" << std::endl;
        return -1;
    }

    std::cout << "\nWebGPU Instance created" << std::endl;

    // request adapter (no surface needed for info dump)
    WGPUAdapter adapter = request_adapter_sync(instance);
    if (!adapter) {
        std::cerr << "Failed to get WebGPU adapter" << std::endl;
        return -1;
    }

    print_adapter_info(adapter);
    print_adapter_features(adapter);
    print_limits(adapter);

    // try creating a device to confirm it works
    WGPUDevice device = request_device_sync(instance, adapter);
    if (device) {
        std::cout << "\nDevice creation: OK" << std::endl;
        wgpuDeviceRelease(device);
    } else {
        std::cout << "\nDevice creation: FAILED" << std::endl;
    }

    // cleanup
    wgpuAdapterRelease(adapter);
    wgpuInstanceRelease(instance);
    glfwTerminate();

    return 0;
}
