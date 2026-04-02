# gl-boilerplate

## GLFW + GLAD

| GL Version  | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| Legacy      | ✅      | ⚠️\*     | ✅         |
| GL 3.3 Core | ✅      | ✅\*\*   | ✅         |
| GL 4.1 Core | ✅      | ✅       | ✅         |
| GL 4.6 Core | ✅      | ❌\*\*\* | ⚠️\*\*\*\* |
| GLES 2.0    | ⚠️†     | ❌       | ⚠️‡        |
| GLES 3.0    | ⚠️†     | ❌       | ⚠️‡        |

```
* Legacy OpenGL not supported on modern macOS (10.14+). GLAD with compatibility profile is required
** Requires forward compatible context
*** macOS only supports up to OpenGL 4.1
**** Requires recent GPU/driver with GL 4.6 support
† Requires proper drivers for GLES support (some of the latest AMD and NVIDIA drivers do)
‡ Mesa provides GLES support through translation layer or direct hardware support if available
```

## GLFW + GLAD2

| GL Version  | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| Legacy      | ✅      | ⚠️\*     | ✅         |
| GL 3.3 Core | ✅      | ✅\*\*   | ✅         |
| GL 4.1 Core | ✅      | ✅       | ✅         |
| GL 4.6 Core | ✅      | ❌\*\*\* | ⚠️\*\*\*\* |
| GLES 2.0    | ⚠️†     | ❌       | ⚠️‡        |
| GLES 3.0    | ⚠️†     | ❌       | ⚠️‡        |

```
* Legacy OpenGL not supported on modern macOS (10.14+). GLAD with compatibility profile is required
** Requires forward compatible context
*** macOS only supports up to OpenGL 4.1
**** Requires recent GPU/driver with GL 4.6 support
† Requires proper drivers for GLES support (some of the latest AMD and NVIDIA drivers do)
‡ Mesa provides GLES support through translation layer or direct hardware support if available
```

## GLFW + GLAD2 (EGL)

These demos are bounded to fail as they explicitly request EGL contexts which these platforms do not natively support. 

| GL Version  | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| GLES 2.0    | ❌      | ❌       | ⚠️†        |
| GLES 3.0    | ❌      | ❌       | ⚠️†        |

```
† Again, Linux survives because Mesa provides a compliant EGL interface
```

### glfw_glad2_egl_200es.cpp

On Windows:
```
GLFW Error 65542: EGL: Library not found
Failed to create GLFW window
```

On macOS:
```
GLFW Error 65542: EGL: Library not found
Failed to create GLFW window
```

On Linux (wsl):
```
EGL Version: 1.5
EGL Vendor: Mesa Project
EGL Version: 1.5
EGL Client APIs: OpenGL OpenGL_ES
GL Vendor: Microsoft Corporation
GL Renderer: D3D12 (AMD Radeon(TM) Graphics)
GL Version: OpenGL ES 3.1 Mesa 23.0.4-0ubuntu1~22.04.1
GLSL Version: OpenGL ES GLSL ES 3.10
Max Viewport Dimensions: 16384x16384
GLAD GLES version: 3.1
```

## GLFW + GLAD2 + ANGLE (EGL)

| GL Version  | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| GLES 2.0    | ✅      | ✅       | ✅         |
| GLES 3.0    | ✅      | ✅       | ✅         |

### glfw_glad2_angle_egl_200es.cpp

On Windows (AMD Radeon, ANGLE D3D11):
```
EGL Version: 1.5
EGL Vendor: Google Inc. (AMD)
EGL Version: 1.5 (ANGLE 2.1.23876 git hash: fffbc739779a)
EGL Client APIs: OpenGL_ES
GL Vendor: Google Inc. (AMD)
GL Renderer: ANGLE (AMD, AMD Radeon(TM) Graphics (0x00001638) Direct3D11 vs_5_0 ps_5_0, D3D11-31.0.21912.1002)
GL Version: OpenGL ES 3.0.0 (ANGLE 2.1.23876 git hash: fffbc739779a)
GLSL Version: OpenGL ES GLSL ES 3.00 (ANGLE 2.1.23876 git hash: fffbc739779a)
Max Viewport Dimensions: 32767x32767
GLAD GLES version: 3.0
```

On macOS (2018 Mac mini):
```
EGL Version: 1.5
EGL Vendor: Google Inc. (Intel Inc.)
EGL Version: 1.5 (ANGLE 2.1.23876 git hash: fffbc739779a)
EGL Client APIs: OpenGL_ES
GL Vendor: Google Inc. (Intel Inc.)
GL Renderer: ANGLE (Intel Inc., Intel(R) UHD Graphics 630, OpenGL 4.1 INTEL-22.5.10)
GL Version: OpenGL ES 3.0.0 (ANGLE 2.1.23876 git hash: fffbc739779a)
GLSL Version: OpenGL ES GLSL ES 3.00 (ANGLE 2.1.23876 git hash: fffbc739779a)
Max Viewport Dimensions: 16384x16384
GLAD GLES version: 3.0
```

On Linux/WSL2 (Mesa EGL, not ANGLE):
```
EGL Version: 1.5
EGL Vendor: Mesa Project
EGL Version: 1.5
EGL Client APIs: OpenGL OpenGL_ES 
GL Vendor: Microsoft Corporation
GL Renderer: D3D12 (AMD Radeon(TM) Graphics)
GL Version: OpenGL ES 3.1 Mesa 23.0.4-0ubuntu1~22.04.1
GLSL Version: OpenGL ES GLSL ES 3.10
Max Viewport Dimensions: 16384x16384
GLAD GLES version: 3.1
```

## raylib + ANGLE

Raylib configured to use OpenGL ES through ANGLE (translates to Metal on macOS).

| Feature     | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| GLES 2.0    | ✅      | ✅       | ✅         |
| GLES 3.0    | ✅      | ✅       | ✅         |
| GLSL 300 es | ✅      | ✅       | ✅         |

Run with: `bazel run --config=angle //ex5-raylib-angle-egl:<target>`

Examples:
- `raylib_angle_basic` - Simple bouncing ball demo
- `raylib_angle_shader_300es` - Custom GLSL 300 es shaders with animated cube

On Windows (AMD Radeon, ANGLE D3D11):
```
INFO: GLAD: OpenGL ES 2.0 loaded successfully
INFO: GL: OpenGL device information:
INFO:     > Vendor:   Google Inc. (AMD)
INFO:     > Renderer: ANGLE (AMD, AMD Radeon(TM) Graphics (0x00001638) Direct3D11 vs_5_0 ps_5_0, D3D11-31.0.21912.1002)
INFO:     > Version:  OpenGL ES 3.0.0 (ANGLE 2.1.23876 git hash: fffbc739779a)
INFO:     > GLSL:     OpenGL ES GLSL ES 3.00 (ANGLE 2.1.23876 git hash: fffbc739779a)
```

On macOS (2018 Mac mini):
```
INFO: GLAD: OpenGL ES 2.0 loaded successfully
INFO: GL: OpenGL device information:
INFO:     > Vendor:   Google Inc. (Intel Inc.)
INFO:     > Renderer: ANGLE (Intel Inc., Intel(R) UHD Graphics 630, OpenGL 4.1 INTEL-23.0.26)
INFO:     > Version:  OpenGL ES 3.0.0 (ANGLE 2.1.23876 git hash: fffbc739779a)
INFO:     > GLSL:     OpenGL ES GLSL ES 3.00 (ANGLE 2.1.23876 git hash: fffbc739779a)
```

On Linux/WSL2 (Mesa, not ANGLE):
```
INFO: GLAD: OpenGL ES 2.0 loaded successfully
INFO: GL: OpenGL device information:
INFO:     > Vendor:   Microsoft Corporation
INFO:     > Renderer: D3D12 (AMD Radeon(TM) Graphics)
INFO:     > Version:  OpenGL ES 3.1 Mesa 23.0.4-0ubuntu1~22.04.1
INFO:     > GLSL:     OpenGL ES GLSL ES 3.10
```

**Note**: Even though it says "OpenGL ES 2.0 loaded", ANGLE/Mesa provides full ES 3.0+ support at runtime.

## GLFW + WebGPU (wgpu-native)

| Backend  | Windows | macOS | Linux      |
| -------- | ------- | ----- | ---------- |
| Vulkan   | ✅      |       | ⚠️\*       |
| Metal    |         |       |            |
| OpenGL   |         |       | ✅ (WSL2)  |

```
* requires proper vulkan drivers (llvmpipe not sufficient)
```

wgpu-native v27.0.4.0 (mozilla's). no GLAD needed -- links directly against `webgpu.h` C API.

Examples:
- `glfw_wgpu_triangle` - rotating colored triangle (WGSL shaders, uniform buffer)
- `glfw_wgpu_info` - dumps adapter info, features, limits

### glfw_wgpu_info

On Windows (GTX 1660 Ti):
```
GLFW Version: 3.4.0
Vulkan supported: yes

WebGPU Instance created

Adapter Information:
  Vendor:       NVIDIA
  Architecture: (null)
  Device:       NVIDIA GeForce GTX 1660 Ti
  Description:  561.17
  Backend:      Vulkan
  Adapter Type: Discrete GPU
  Vendor ID:    0x10de
  Device ID:    0x2191

Supported Features (36):
  - DepthClipControl
  - Depth32FloatStencil8
  - TextureCompressionBC
  - TimestampQuery
  - IndirectFirstInstance
  - ShaderF16
  - RG11B10UfloatRenderable
  - BGRA8UnormStorage
  - Float32Filterable
  - DualSourceBlending
  - (vendor/ext 0x30001)
  - (vendor/ext 0x30002)
  - (vendor/ext 0x30004)
  - (vendor/ext 0x30005)
  - (vendor/ext 0x30006)
  - (vendor/ext 0x30007)
  - (vendor/ext 0x30008)
  - (vendor/ext 0x30009)
  - (vendor/ext 0x3000a)
  - (vendor/ext 0x3000b)
  - (vendor/ext 0x30025)
  - (vendor/ext 0x30024)
  - (vendor/ext 0x3000e)
  - (vendor/ext 0x3000f)
  - (vendor/ext 0x30013)
  - (vendor/ext 0x30014)
  - (vendor/ext 0x30015)
  - (vendor/ext 0x3001a)
  - (vendor/ext 0x3001d)
  - (vendor/ext 0x30026)
  - (vendor/ext 0x3001e)
  - (vendor/ext 0x3001f)
  - (vendor/ext 0x30020)
  - (vendor/ext 0x30021)
  - (vendor/ext 0x30022)
  - (vendor/ext 0x30023)

Limits:
  maxTextureDimension1D:          32768
  maxTextureDimension2D:          32768
  maxTextureDimension3D:          16384
  maxTextureArrayLayers:          2048
  maxBindGroups:                  8
  maxBindingsPerBindGroup:        1000
  maxBufferSize:                  4503599627370496
  maxVertexBuffers:               16
  maxVertexAttributes:            32
  maxVertexBufferArrayStride:     2048
  maxSampledTexturesPerShaderStage: 1048576
  maxSamplersPerShaderStage:      1048576
  maxStorageBuffersPerShaderStage: 1048576
  maxStorageTexturesPerShaderStage: 1048576
  maxUniformBuffersPerShaderStage: 1048576
  maxUniformBufferBindingSize:    65536
  maxStorageBufferBindingSize:    2147483647
  maxComputeWorkgroupSizeX:       1024
  maxComputeWorkgroupSizeY:       1024
  maxComputeWorkgroupSizeZ:       64
  maxComputeWorkgroupsPerDimension: 65535
  maxColorAttachments:            8

Device creation: OK
```

On Linux/WSL2 (AMD Radeon via Mesa D3D12):
```
GLFW Version: 3.4.0
Vulkan supported: yes

WebGPU Instance created

Adapter Information:
  Vendor:       
  Architecture: (null)
  Device:       D3D12 (AMD Radeon(TM) Graphics)
  Description:  4.2 (Core Profile) Mesa 23.0.4-0ubuntu1~22.04.1
  Backend:      OpenGL
  Adapter Type: Unknown
  Vendor ID:    0x0
  Device ID:    0x0

Supported Features (17):
  - DepthClipControl
  - Depth32FloatStencil8
  - TextureCompressionBC
  - TextureCompressionETC2
  - TextureCompressionASTC
  - TimestampQuery
  - IndirectFirstInstance
  - DualSourceBlending
  - (vendor/ext 0x30001)
  - (vendor/ext 0x30002)
  - (vendor/ext 0x30005)
  - (vendor/ext 0x30025)
  - (vendor/ext 0x30024)
  - (vendor/ext 0x30013)
  - (vendor/ext 0x30014)
  - (vendor/ext 0x3001f)
  - (vendor/ext 0x30020)

Limits:
  maxTextureDimension1D:          16384
  maxTextureDimension2D:          16384
  maxTextureDimension3D:          2048
  maxTextureArrayLayers:          2048
  maxBindGroups:                  8
  maxBindingsPerBindGroup:        65535
  maxBufferSize:                  2147483647
  maxVertexBuffers:               16
  maxVertexAttributes:            16
  maxVertexBufferArrayStride:     2048
  maxSampledTexturesPerShaderStage: 16
  maxSamplersPerShaderStage:      16
  maxStorageBuffersPerShaderStage: 16
  maxStorageTexturesPerShaderStage: 32
  maxUniformBuffersPerShaderStage: 12
  maxUniformBufferBindingSize:    65536
  maxStorageBufferBindingSize:    134217728
  maxComputeWorkgroupSizeX:       1024
  maxComputeWorkgroupSizeY:       1024
  maxComputeWorkgroupSizeZ:       64
  maxComputeWorkgroupsPerDimension: 65535
  maxColorAttachments:            8

Device creation: OK
```

## GLAD2 + SwANGLE

OpenGL ES on SwiftShader Vulkan

From [SwiftShader](https://github.com/google/swiftshader) repo:

SwiftShader is a high-performance CPU-based implementation of the Vulkan 1.3 graphics API. Its goal is to provide hardware independence for advanced 3D graphics.

> NOTE: The [ANGLE](http://angleproject.org/) project can be used to achieve a layered implementation of OpenGL ES 3.1 (aka. "SwANGLE").

| GL Version  | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| GLES 2.0    |         |          |            |
| GLES 3.0    |         |          |            |
| GLES 3.1    |         |          |            |

## OpenCL (or OpenCL ES) on ANGLE

| CL Version  | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| Legacy      |         |          |            |
