# raylib + ANGLE Examples

These examples demonstrate using raylib with ANGLE (OpenGL ES via Metal on macOS).

Based on: https://medium.com/@grplyler/building-and-linking-googles-angle-with-raylib-on-macos-67b07cd380a3

## Configuration

Raylib is configured to use:
- OpenGL ES 2.0 API (GRAPHICS_API_OPENGL_ES2)
- GLFW for window management  
- GLAD2 for OpenGL ES function loading
- ANGLE backend (translates OpenGL ES to Metal on macOS)

When running, you should see output like:
```
INFO: GLAD: OpenGL ES 2.0 loaded successfully
INFO: GL: OpenGL device information:
INFO:     > Vendor:   Google Inc. (Intel Inc.)
INFO:     > Renderer: ANGLE (Intel Inc., Intel(R) UHD Graphics 630, OpenGL 4.1 INTEL-23.0.26)
INFO:     > Version:  OpenGL ES 3.0.0 (ANGLE 2.1.23876 git hash: fffbc739779a)
INFO:     > GLSL:     OpenGL ES GLSL ES 3.00 (ANGLE 2.1.23876 git hash: fffbc739779a)
```

The key indicator that ANGLE is being used is **"Vendor: Google Inc."** and **"Renderer: ANGLE"**.

## Running on macOS

Use the `--config=angle` flag to set the library path for ANGLE:

```bash
bazel run --config=angle //ex5-raylib-angle-egl:raylib_angle_basic
bazel run --config=angle //ex5-raylib-angle-egl:raylib_angle_shader_300es
```

## Building

Standard build works:

```bash
bazel build //ex5-raylib-angle-egl:raylib_angle_basic
bazel build //ex5-raylib-angle-egl:raylib_angle_shader_300es
```

## Examples

- `raylib_angle_basic.c` - Simple bouncing ball demo
- `raylib_angle_shader_300es.c` - GLSL 300 es custom shader demo with animated wavy cube

## Technical Details

The raylib-angle target is defined in `third_party/raylib-latest.BUILD` and uses:
- GLFW 3.4 for windowing (with native context, not EGL)
- ANGLE libraries (libEGL.dylib, libGLESv2.dylib) for OpenGL ES implementation
- GLAD2 for OpenGL ES function loading
- GRAPHICS_API_OPENGL_ES2 define (enables ES 2.0 and 3.0 support)

This configuration allows raylib to run with ANGLE's OpenGL ES backend on macOS, which translates OpenGL ES calls to Metal for better performance on modern Macs.
