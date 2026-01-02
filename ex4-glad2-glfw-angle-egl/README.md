# ANGLE Examples

These examples use ANGLE (Almost Native Graphics Layer Engine) for OpenGL ES rendering via EGL.

## Running on macOS

Due to dynamic library loading requirements, you must use the `--config=angle` flag:

```bash
bazel run --config=angle //ex4-glad2-glfw-angle-egl:glfw_glad2_angle_egl_300es
bazel run --config=angle //ex4-glad2-glfw-angle-egl:glfw_glad2_angle_egl_200es
```

This configuration sets `DYLD_LIBRARY_PATH` to allow the runtime linker to find the ANGLE libraries.

## Building

Standard build works without special flags:

```bash
bazel build //ex4-glad2-glfw-angle-egl:glfw_glad2_angle_egl_300es
```

## Running the binary directly

If you want to run the built binary directly (not via `bazel run`), you need to set the library path:

```bash
cd bazel-bin/ex4-glad2-glfw-angle-egl/glfw_glad2_angle_egl_300es.runfiles/_main
DYLD_LIBRARY_PATH=third_party/angle ./ex4-glad2-glfw-angle-egl/glfw_glad2_angle_egl_300es
```
