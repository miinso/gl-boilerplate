##
#  glfw3webgpu - GLFW <-> WebGPU surface adapter
##
cc_library(
    name = "glfw3webgpu",
    srcs = ["glfw3webgpu.c"],
    hdrs = ["glfw3webgpu.h"],
    includes = ["."],
    copts = select({
        "@platforms//os:macos": ["-x", "objective-c"],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@platforms//os:macos": [
            "-framework Cocoa",
            "-framework QuartzCore",
        ],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/wgpu-native",
        "@glfw2",
    ],
)
