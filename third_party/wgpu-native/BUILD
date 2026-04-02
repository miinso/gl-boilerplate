##
#  wgpu-native v27.0.4.0
#  prebuilt libs fetched via http_archive per platform
##
cc_library(
    name = "wgpu-native",
    linkopts = select({
        "@platforms//os:windows": [
            "-DEFAULTLIB:d3dcompiler.lib",
            "-DEFAULTLIB:dxgi.lib",
            "-DEFAULTLIB:d3d12.lib",
            # static link pulls in rust std + windows crate deps
            "-DEFAULTLIB:ws2_32.lib",
            "-DEFAULTLIB:userenv.lib",
            "-DEFAULTLIB:ntdll.lib",
            "-DEFAULTLIB:opengl32.lib",
            "-DEFAULTLIB:oleaut32.lib",
            "-DEFAULTLIB:ole32.lib",
            "-DEFAULTLIB:bcrypt.lib",
            "-DEFAULTLIB:runtimeobject.lib",
            "-DEFAULTLIB:propsys.lib",
        ],
        "@platforms//os:linux": [
            "-lpthread",
            "-ldl",
            "-lm",
        ],
        "@platforms//os:macos": [
            "-framework Metal",
            "-framework QuartzCore",
            "-framework Foundation",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = select({
        "@platforms//os:windows": [
            "@wgpu_native_win_x64//:headers",
            "@wgpu_native_win_x64//:lib",
        ],
        "@platforms//os:linux": [
            "@wgpu_native_linux_x64//:headers",
            "@wgpu_native_linux_x64//:lib",
        ],
        "@bazel_tools//src/conditions:darwin_arm64": [
            "@wgpu_native_macos_arm64//:headers",
            "@wgpu_native_macos_arm64//:lib",
        ],
        "@bazel_tools//src/conditions:darwin_x86_64": [
            "@wgpu_native_macos_x64//:headers",
            "@wgpu_native_macos_x64//:lib",
        ],
    }),
)
