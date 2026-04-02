load("@rules_cc//cc:cc_library.bzl", "cc_library")

##
#  ANGLE static (godot-angle-static, chromium/7578)
#  EGL + GLES headers reused from //third_party/angle/include
##
cc_library(
    name = "angle-static",
    linkopts = select({
        "@platforms//os:linux": [
            "-lpthread",
            "-ldl",
            "-lm",
            "-lvulkan",
        ],
        "@platforms//os:macos": [
            "-framework Metal",
            "-framework IOSurface",
            "-framework QuartzCore",
            "-framework Cocoa",
            "-framework Foundation",
        ],
        "@platforms//os:windows": [
            "-DEFAULTLIB:d3d11.lib",
            "-DEFAULTLIB:dxgi.lib",
            "-DEFAULTLIB:d3dcompiler.lib",
            "-DEFAULTLIB:gdi32.lib",
            "-DEFAULTLIB:user32.lib",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/angle",
    ] + select({
        "@bazel_tools//src/conditions:darwin_arm64": [
            "@angle_static_macos_arm64//:libANGLE",
            "@angle_static_macos_arm64//:libEGL",
            "@angle_static_macos_arm64//:libGLES",
        ],
        "@bazel_tools//src/conditions:darwin_x86_64": [
            "@angle_static_macos_x64//:libANGLE",
            "@angle_static_macos_x64//:libEGL",
            "@angle_static_macos_x64//:libGLES",
        ],
        "@platforms//os:linux": [
            "@angle_static_linux_x64//:libANGLE",
            "@angle_static_linux_x64//:libEGL",
            "@angle_static_linux_x64//:libGLES",
        ],
        "@platforms//os:windows": [
            "@angle_static_win_x64//:libANGLE",
            "@angle_static_win_x64//:libEGL",
            "@angle_static_win_x64//:libGLES",
        ],
    }),
)

exports_files(
    glob(["*.BUILD"]),
    visibility = ["//visibility:public"],
)
