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
| GLES 2.0    |         | ✅        |            |
| GLES 3.0    |         | ✅        |            |

### glfw_glad2_angle_egl_200es.cpp

On Windows:
```
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

On Linux:
```
```

## raylib + ANGLE

Raylib configured to use OpenGL ES through ANGLE (translates to Metal on macOS).

| Feature     | Windows | macOS    | Linux      |
| ----------- | ------- | -------- | ---------- |
| GLES 2.0    |         | ✅       |            |
| GLES 3.0    |         | ✅       |            |
| GLSL 300 es |         | ✅       |            |

Run with: `bazel run --config=angle //ex5-raylib-angle-egl:<target>`

Examples:
- `raylib_angle_basic` - Simple bouncing ball demo
- `raylib_angle_shader_300es` - Custom GLSL 300 es shaders with animated cube

On macOS (2018 Mac mini):
```
INFO: GLAD: OpenGL ES 2.0 loaded successfully
INFO: GL: OpenGL device information:
INFO:     > Vendor:   Google Inc. (Intel Inc.)
INFO:     > Renderer: ANGLE (Intel Inc., Intel(R) UHD Graphics 630, OpenGL 4.1 INTEL-23.0.26)
INFO:     > Version:  OpenGL ES 3.0.0 (ANGLE 2.1.23876 git hash: fffbc739779a)
INFO:     > GLSL:     OpenGL ES GLSL ES 3.00 (ANGLE 2.1.23876 git hash: fffbc739779a)
```

**Note**: Even though it says "OpenGL ES 2.0 loaded", ANGLE provides full ES 3.0 support at runtime.

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
